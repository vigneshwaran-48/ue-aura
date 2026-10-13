#include "Inventory/UI/Grid/AuraInventoryGridWidget.h"

#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "GameFramework/Pawn.h"
#include "Inventory/AuraInventoryComponent.h"
#include "Inventory/Fragments/AuraItemFragment_Size.h"
#include "Inventory/Layouts/AuraGridInventoryLayout.h"
#include "Inventory/UI/Grid/AuraInventoryDragDropOperation.h"
#include "Inventory/UI/Grid/AuraInventoryGhostWidget.h"
#include "Inventory/UI/Grid/AuraInventoryItemWidget.h"
#include "Inventory/UI/Grid/AuraInventorySlotWidget.h"
#include "Inventory/AuraItemHandle.h"
#include "Inventory/UI/Grid/AuraInventoryInteractController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/DataTable.h"
#include "Input/CommonUIInputTypes.h"
#include "UI/AuraUIManagerComponent.h"
#include "AuraGameplayTags.h"
#include "UI/AuraUIContext.h"
#include "UI/AuraUIContextSubsystem.h"
#include "Inventory/Actions/AuraItemAction.h"
#include "Inventory/UI/Actions/AuraItemActionMenuWidget.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/NavigationConfig.h"

void UAuraInventoryGridWidget::NativeConstruct() {
	Super::NativeConstruct();
}

void UAuraInventoryGridWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	DisableSlateNavigation();

	InventoryUIContext = FAuraUIContext{};
	bHasInventoryUIContext = false;

	ConsumeUIContext(TAG_UI_Inventory, InventoryUIContext);

	BuildGrid();
	PopulateItems();

	SetKeyboardFocus();

	if (!InteractionController) {
		InteractionController =
			NewObject<UAuraInventoryInteractController>(this);
	}

	InteractionController->Initialize(this);
	InteractionController->InitializeSelection();

	RegisterAction(MoveInputActionData, FSimpleDelegate::CreateUObject(this, &ThisClass::OnMoveSelected));
	RegisterAction(CancelInputActionData, FSimpleDelegate::CreateUObject(this, &ThisClass::OnCancelPressed));
	RegisterAction(CloseInventoryInputActionRowHandle, FSimpleDelegate::CreateUObject(this, &ThisClass::OnCloseRequested), true);

	RegisterAction(MoveUpInputActionData, FSimpleDelegate::CreateLambda([this]()
		{
			MoveSelection(FIntPoint(0, -1));
		}));

	RegisterAction(MoveDownInputActionData, FSimpleDelegate::CreateLambda([this]()
		{
			MoveSelection(FIntPoint(0, 1));
		}));

	RegisterAction(MoveLeftInputActionData, FSimpleDelegate::CreateLambda([this]()
		{
			MoveSelection(FIntPoint(-1, 0));
		}));

	RegisterAction(MoveRightInputActionData, FSimpleDelegate::CreateLambda([this]()
		{
			MoveSelection(FIntPoint(1, 0));
		}));

	RegisterAction(
		OpenItemActionsInputActionData,
		FSimpleDelegate::CreateUObject(
			this,
			&ThisClass::OnOpenItemActionsRequested));
}

void UAuraInventoryGridWidget::NativeOnDeactivated()
{

	RestoreSlateNavigation();

	if (InteractionController) {
		InteractionController->CancelItemMove();
		InteractionController->ClearSelection();
	}

	UWidgetBlueprintLibrary::CancelDragDrop();

	CloseItemActionMenu();

	Super::NativeOnDeactivated();
}

void UAuraInventoryGridWidget::RegisterAction(const FDataTableRowHandle& ActionHandle, const FSimpleDelegate& Delegate, bool bConsumeInput)
{
	if (!ActionHandle.IsNull())
	{
		FBindUIActionArgs BindArgs(ActionHandle, Delegate);

		BindArgs.bIsPersistent = false;
		BindArgs.bConsumeInput = bConsumeInput;
		BindArgs.bDisplayInActionBar = true;

		RegisterUIActionBinding(BindArgs);
	}
}

void UAuraInventoryGridWidget::MoveSelection(const FIntPoint& Direction)
{
	if (ActionMenuWidget && ActionMenuWidget->IsActivated()) {
		// If context menu is open stop navigation in the inventory grid.
		return;
	}

	if (InteractionController)
	{
		InteractionController->MoveSelection(Direction);
	}
}

void UAuraInventoryGridWidget::OnMoveSelected()
{
	
	if (InteractionController)
	{
		InteractionController->HandleConfirm();
	}
}

void UAuraInventoryGridWidget::OnCancelPressed()
{
	if (InteractionController)
	{
		InteractionController->HandleCancel();
	}
}

void UAuraInventoryGridWidget::OnCloseRequested()
{
	if (AActor* Owner = GetOwningPlayerPawn())
	{
		if (UAuraUIManagerComponent* UIManager = Owner->FindComponentByClass<UAuraUIManagerComponent>())
		{
			UIManager->ToggleUI(TAG_UI_Inventory);
		}
	}
}

void UAuraInventoryGridWidget::UpdateControllerSelectionVisual()
{
	for (const auto& Pair : GridSlots) {
		if (Pair.Value) {
			Pair.Value->SetSelected(false);
		}
	}

	for (const auto& Pair : ItemWidgets) {
		if (Pair.Value) {
			Pair.Value->SetSelected(false);
		}
	}

	if (!InteractionController ||
		!InteractionController->HasSelection() ||
		InteractionController->IsMovingItem()) {
		return;
	}

	UAuraInventoryComponent* Inventory =
		GetInventoryComponent();

	if (!Inventory) {
		return;
	}

	UAuraGridInventoryLayout* Layout =
		Cast<UAuraGridInventoryLayout>(
			Inventory->GetLayout());

	if (!Layout) {
		return;
	}

	const FIntPoint SelectedCell =
		InteractionController->GetSelectedCell();

	FAuraItemHandle Handle;

	if (Layout->GetItemAtCell(SelectedCell, Handle)) {
		if (TObjectPtr<UAuraInventoryItemWidget>* ItemWidget =
			ItemWidgets.Find(Handle)) {
			if (*ItemWidget) {
				(*ItemWidget)->SetSelected(true);
			}
		}

		return;
	}

	if (TObjectPtr<UAuraInventorySlotWidget>* SlotWidget =
		GridSlots.Find(SelectedCell)) {
		if (*SlotWidget) {
			(*SlotWidget)->SetSelected(true);
		}
	}
}

void UAuraInventoryGridWidget::UpdateControllerMoveVisual()
{
	if (!InteractionController ||
		!InteractionController->IsMovingItem()) {
		return;
	}

	SetGhostCell(
		InteractionController->GetMoveState().CurrentPosition);
}

void UAuraInventoryGridWidget::ClearControllerMoveVisual()
{
	if (GhostWidget) {
		GhostWidget->SetVisibility(
			ESlateVisibility::Collapsed);
	}
}

void UAuraInventoryGridWidget::SetGhostCell(
	FIntPoint Cell,
	bool bShowGhost)
{
	UAuraInventoryComponent* Inventory =
		GetInventoryComponent();

	if (!Inventory || !GhostWidgetClass || !InteractionController) {
		return;
	}

	UAuraGridInventoryLayout* Layout =
		Cast<UAuraGridInventoryLayout>(
			Inventory->GetLayout());

	if (!Layout) {
		return;
	}

	if (!GhostWidget) {
		GhostWidget =
			CreateWidget<UAuraInventoryGhostWidget>(
				this,
				GhostWidgetClass);

		if (!GhostWidget) {
			return;
		}

		UCanvasPanelSlot* GhostSlot =
			ItemCanvas->AddChildToCanvas(GhostWidget);

		if (GhostSlot) {
			GhostSlot->SetZOrder(-1);
		}
	}

	const float CellSize = Layout->GetCellSize();

	const FIntPoint ItemSize =
		InteractionController->GetMoveState().ItemSize;

	UCanvasPanelSlot* CanvasSlot =
		Cast<UCanvasPanelSlot>(GhostWidget->Slot);

	if (!CanvasSlot) {
		return;
	}

	CanvasSlot->SetPosition(
		FVector2D(
			Cell.X * CellSize,
			Cell.Y * CellSize));

	CanvasSlot->SetSize(
		FVector2D(
			ItemSize.X * CellSize,
			ItemSize.Y * CellSize));

	const bool bCanPlace =
		InteractionController->CanPlaceAt(Cell);

	GhostWidget->SetGhostState(
		bCanPlace
		? EGhostWidgetState::Valid
		: EGhostWidgetState::Invalid);

	GhostWidget->SetVisibility(
		bShowGhost
		? ESlateVisibility::HitTestInvisible
		: ESlateVisibility::Collapsed);
}

UAuraInventoryComponent* UAuraInventoryGridWidget::GetInventoryComponent()
const {
	APawn* Pawn = GetOwningPlayerPawn();
	if (!Pawn) {
		return nullptr;
	}

	return Pawn->FindComponentByClass<UAuraInventoryComponent>();
}

void UAuraInventoryGridWidget::BuildGrid() {
	if (!GridPanel || !SlotWidgetClass) {
		return;
	}

	UAuraInventoryComponent* Inventory = GetInventoryComponent();
	if (!Inventory) {
		UE_LOG(LogTemp, Warning, TEXT("No InventoryComponent"));
		return;
	}

	UAuraGridInventoryLayout* GridLayout =
		Cast<UAuraGridInventoryLayout>(Inventory->GetLayout());

	if (!GridLayout) {
		UE_LOG(LogTemp, Warning, TEXT("Layout not grid layout"));
		return;
	}

	const int32 Rows = GridLayout->GetRows();
	const int32 Cols = GridLayout->GetColumns();
	const float CellSize = GridLayout->GetCellSize();

	GridPanel->ClearChildren();

	if (GridSizeBox) {
		GridSizeBox->SetWidthOverride(Cols * CellSize);
		GridSizeBox->SetHeightOverride(Rows * CellSize);
	}

	GridSlots.Empty();

	for (int32 Row = 0; Row < Rows; ++Row) {
		for (int32 Col = 0; Col < Cols; ++Col) {
			UAuraInventorySlotWidget* NewSlot =
				CreateWidget<UAuraInventorySlotWidget>(this, SlotWidgetClass);

			if (!NewSlot) {
				continue;
			}

			NewSlot->SetSlotSize(CellSize);

			UUniformGridSlot* GridSlot =
				GridPanel->AddChildToUniformGrid(NewSlot, Row, Col);

			if (GridSlot) {
				GridSlot->SetHorizontalAlignment(HAlign_Fill);
				GridSlot->SetVerticalAlignment(VAlign_Fill);
			}

			GridSlots.Add(
				FIntPoint(Col, Row),
				NewSlot);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Grid built %dx%d CellSize %.1f"), Rows, Cols,
		CellSize);
}

void UAuraInventoryGridWidget::PopulateItems() {
	if (!ItemCanvas || !ItemWidgetClass) {
		return;
	}

	TArray<UWidget*> Children = ItemCanvas->GetAllChildren();
	for (UWidget* Child : Children) {
		if (Child != Cast<UWidget>(GhostWidget)) {
			Child->RemoveFromParent();
		}
	}

	ItemWidgets.Empty();

	UAuraInventoryComponent* Inventory = GetInventoryComponent();
	if (!Inventory) {
		return;
	}

	UAuraGridInventoryLayout* Layout =
		Cast<UAuraGridInventoryLayout>(Inventory->GetLayout());

	if (!Layout) {
		return;
	}

	const float CellSize = Layout->GetCellSize();

	TArray<FAuraItemHandle> Handles;
	Layout->GetAllItems(Handles);

	for (const FAuraItemHandle& Handle : Handles) {
		const FAuraItemInstance* Item = Inventory->FindItem(Handle);
		if (!Item) {
			continue;
		}

		FIntPoint Position;
		if (!Layout->GetItemPosition(Handle, Position)) {
			continue;
		}

		const FIntPoint Size = Layout->GetItemSize(Handle);

		UAuraInventoryItemWidget* ItemWidget =
			CreateWidget<UAuraInventoryItemWidget>(this, ItemWidgetClass);

		if (!ItemWidget) {
			continue;
		}

		ItemWidget->InitFromItem(*Item, Handle, Layout->GetCellSize());
		ItemWidget->OwningGrid = this;

		UCanvasPanelSlot* CanvasSlot = ItemCanvas->AddChildToCanvas(ItemWidget);

		if (!CanvasSlot) {
			continue;
		}

		CanvasSlot->SetAutoSize(false);

		CanvasSlot->SetPosition(
			FVector2D(Position.X * CellSize, Position.Y * CellSize));

		CanvasSlot->SetSize(FVector2D(Size.X * CellSize, Size.Y * CellSize));

		ItemWidgets.Add(Handle, ItemWidget);
	}
}

bool UAuraInventoryGridWidget::NativeOnDrop(
	const FGeometry& InGeometry,
	const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UAuraInventoryDragDropOperation* DragOp =
		Cast<UAuraInventoryDragDropOperation>(InOperation);

	if (!DragOp || !InteractionController ||
		!InteractionController->IsMovingItem()) {
		return false;
	}

	if (InteractionController->CanPlaceAt(
		InteractionController->GetMoveState().CurrentPosition)) {
		return InteractionController->CommitItemMove();
	}

	InteractionController->CancelItemMove();
	return true;
}

bool UAuraInventoryGridWidget::NativeOnDragOver(
	const FGeometry& InGeometry,
	const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UAuraInventoryDragDropOperation* DragOp =
		Cast<UAuraInventoryDragDropOperation>(InOperation);

	if (!DragOp ||
		!InteractionController ||
		!InteractionController->IsMovingItem()) {
		return false;
	}

	const FVector2D LocalMousePosition =
		InGeometry.AbsoluteToLocal(
			InDragDropEvent.GetScreenSpacePosition());

	const FIntPoint Cell =
		InteractionController->GetCellFromMousePosition(
			LocalMousePosition,
			DragOp->DragOffset);

	InteractionController->UpdateItemMove(Cell);

	return true;
}

void UAuraInventoryGridWidget::NativeOnDragLeave(
	const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(
		InDragDropEvent,
		InOperation);
}

void UAuraInventoryGridWidget::NativeOnDragCancelled(
	const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UAuraInventoryDragDropOperation* DragOp =
		Cast<UAuraInventoryDragDropOperation>(InOperation);

	if (DragOp && InteractionController &&
		InteractionController->IsMovingItem()) {
		InteractionController->CancelItemMove();
	}

	Super::NativeOnDragCancelled(
		InDragDropEvent,
		InOperation);
}

void UAuraInventoryGridWidget::BeginItemMoveVisual(
	const FAuraItemHandle& Handle)
{
	if (!ItemCanvas)
		return;

	if (TObjectPtr<UAuraInventoryItemWidget>* ExistingWidget =
		ItemWidgets.Find(Handle)) {
		MovingItemWidget = *ExistingWidget;
		ItemWidgets.Remove(Handle);
	}

	if (!MovingItemWidget)
		return;

	MovingItemWidget->SetVisibility(
		ESlateVisibility::HitTestInvisible);

	if (UCanvasPanelSlot* CanvasSlot =
		Cast<UCanvasPanelSlot>(MovingItemWidget->Slot)) {
		CanvasSlot->SetZOrder(1);
	}
}

void UAuraInventoryGridWidget::UpdateItemMoveVisual()
{
	if (!MovingItemWidget ||
		!InteractionController ||
		!InteractionController->IsMovingItem()) {
		return;
	}

	UAuraInventoryComponent* Inventory =
		GetInventoryComponent();

	if (!Inventory)
		return;

	UAuraGridInventoryLayout* Layout =
		Cast<UAuraGridInventoryLayout>(
			Inventory->GetLayout());

	if (!Layout)
		return;

	const FIntPoint Position =
		InteractionController->GetMoveState().CurrentPosition;

	const FIntPoint Size =
		InteractionController->GetMoveState().ItemSize;

	const float CellSize =
		Layout->GetCellSize();

	if (UCanvasPanelSlot* CanvasSlot =
		Cast<UCanvasPanelSlot>(MovingItemWidget->Slot)) {
		CanvasSlot->SetPosition(
			FVector2D(
				Position.X * CellSize,
				Position.Y * CellSize));

		CanvasSlot->SetSize(
			FVector2D(
				Size.X * CellSize,
				Size.Y * CellSize));
	}
}

void UAuraInventoryGridWidget::ClearItemMoveVisual()
{
	MovingItemWidget = nullptr;
}

// This method is for handling cases where the item is dragged outside of the grid and dropped with mouse.
bool UAuraInventoryGridWidget::HandleDropOutsideGrid()
{
	if (!InteractionController ||
		!InteractionController->IsMovingItem()) {
		return false;
	}

	InteractionController->CancelItemMove();

	return true;
}

void UAuraInventoryGridWidget::OnOpenItemActionsRequested()
{
	if (!InteractionController ||
		InteractionController->IsMovingItem() ||
		!ItemCanvas ||
		!ActionMenuWidgetClass)
	{
		return;
	}

	FAuraItemHandle ItemHandle;

	if (!InteractionController->GetSelectedItem(ItemHandle))
		return;

	const TArray<FAuraItemAction> AvailableActions =
		InteractionController->GetAvailableActions(
			ItemHandle,
			InventoryUIContext);

	if (AvailableActions.IsEmpty())
		return;

	TObjectPtr<UAuraInventoryItemWidget>* ItemWidgetPtr =
		ItemWidgets.Find(ItemHandle);

	if (!ItemWidgetPtr || !*ItemWidgetPtr)
		return;

	UCanvasPanelSlot* ItemSlot =
		Cast<UCanvasPanelSlot>((*ItemWidgetPtr)->Slot);

	if (!ItemSlot)
		return;

	CloseItemActionMenu();

	ActionMenuWidget = CreateWidget<UAuraItemActionMenuWidget>(
		GetOwningPlayer(),
		ActionMenuWidgetClass);

	if (!ActionMenuWidget)
		return;

	ActionMenuWidget->SetActions(
		ItemHandle,
		AvailableActions);

	ActionMenuWidget->OnActionSelected.BindUObject(
		this,
		&ThisClass::HandleContextMenuActionSelected);

	ActionMenuWidget->OnMenuClosed.BindUObject(
		this,
		&ThisClass::HandleContextMenuClosed);

	UCanvasPanelSlot* MenuSlot =
		ItemCanvas->AddChildToCanvas(ActionMenuWidget);

	if (!MenuSlot)
	{
		ActionMenuWidget = nullptr;
		return;
	}

	MenuSlot->SetAutoSize(true);
	MenuSlot->SetZOrder(100);

	ActionMenuWidget->ForceLayoutPrepass();

	const FVector2D ItemPosition = ItemSlot->GetPosition();
	const FVector2D ItemSize = ItemSlot->GetSize();
	const FVector2D MenuSize = ActionMenuWidget->GetDesiredSize();
	const FVector2D CanvasSize =
		ItemCanvas->GetCachedGeometry().GetLocalSize();

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Action menu: DesiredSize=(%.1f, %.1f), CanvasSize=(%.1f, %.1f), Entries=%d"),
		MenuSize.X,
		MenuSize.Y,
		CanvasSize.X,
		CanvasSize.Y,
		AvailableActions.Num());

	constexpr float Gap = 4.0f;

	FVector2D Position(
		ItemPosition.X + ItemSize.X + Gap,
		ItemPosition.Y + ItemSize.Y + Gap);

	if (Position.X + MenuSize.X > CanvasSize.X)
		Position.X = ItemPosition.X - MenuSize.X - Gap;

	if (Position.Y + MenuSize.Y > CanvasSize.Y)
		Position.Y = ItemPosition.Y + ItemSize.Y - MenuSize.Y;

	Position.X = FMath::Clamp(
		Position.X,
		0.0f,
		FMath::Max(0.0f, CanvasSize.X - MenuSize.X));

	Position.Y = FMath::Clamp(
		Position.Y,
		0.0f,
		FMath::Max(0.0f, CanvasSize.Y - MenuSize.Y));

	MenuSlot->SetPosition(Position);

	ActionMenuWidget->ActivateWidget();
}

void UAuraInventoryGridWidget::HandleContextMenuActionSelected(
	FAuraItemHandle ItemHandle,
	FAuraItemAction Action)
{
	UE_LOG(
		LogTemp,
		Log,
		TEXT("Selected action '%s' for item."),
		*Action.DisplayName.ToString());

	CloseItemActionMenu();
}

void UAuraInventoryGridWidget::HandleContextMenuClosed()
{
	CloseItemActionMenu();
}

void UAuraInventoryGridWidget::CloseItemActionMenu()
{
	if (!ActionMenuWidget)
		return;

	UAuraItemActionMenuWidget* Menu = ActionMenuWidget;
	ActionMenuWidget = nullptr;

	if (Menu->IsActivated())
		Menu->DeactivateWidget();

	Menu->RemoveFromParent();

	SetKeyboardFocus();
}

void UAuraInventoryGridWidget::DisableSlateNavigation()
{
	if (!FSlateApplication::IsInitialized() ||
		bHasSavedNavigationConfig)
		return;

	FSlateApplication& SlateApp = FSlateApplication::Get();

	PreviousNavigationConfig = SlateApp.GetNavigationConfig();
	bHasSavedNavigationConfig = PreviousNavigationConfig.IsValid();

	TSharedRef<FNavigationConfig> NavigationConfig =
		MakeShared<FNavigationConfig>();

	NavigationConfig->KeyEventRules.Empty();
	NavigationConfig->AnalogHorizontalKey = EKeys::Invalid;
	NavigationConfig->AnalogVerticalKey = EKeys::Invalid;

	SlateApp.SetNavigationConfig(NavigationConfig);
}

void UAuraInventoryGridWidget::RestoreSlateNavigation()
{
	if (!FSlateApplication::IsInitialized() ||
		!bHasSavedNavigationConfig ||
		!PreviousNavigationConfig.IsValid())
		return;

	FSlateApplication::Get().SetNavigationConfig(
		PreviousNavigationConfig.ToSharedRef());

	PreviousNavigationConfig.Reset();
	bHasSavedNavigationConfig = false;
}