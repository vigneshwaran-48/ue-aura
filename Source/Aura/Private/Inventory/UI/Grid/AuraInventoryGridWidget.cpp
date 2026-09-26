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

void UAuraInventoryGridWidget::NativeConstruct() {
	Super::NativeConstruct();
}

void UAuraInventoryGridWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	BuildGrid();
	PopulateItems();

	SetKeyboardFocus();

	if (!InteractionController) {
		InteractionController =
			NewObject<UAuraInventoryInteractController>(this);
	}

	InteractionController->Initialize(this);
	InteractionController->InitializeSelection();
}

void UAuraInventoryGridWidget::NativeOnDeactivated()
{
	if (InteractionController) {
		InteractionController->CancelItemMove();
		InteractionController->ClearSelection();
	}

	Super::NativeOnDeactivated();
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

// For now handling direct controller input, but it should be changed through the CommonUI.
FReply UAuraInventoryGridWidget::NativeOnKeyDown(
	const FGeometry& InGeometry,
	const FKeyEvent& InKeyEvent)
{
	if (!InteractionController) {
		return Super::NativeOnKeyDown(
			InGeometry,
			InKeyEvent);
	}

	const FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::Up || Key == EKeys::Gamepad_DPad_Up)
	{
		InteractionController->MoveSelection(
			FIntPoint(0, -1));

		return FReply::Handled();
	}

	if (Key == EKeys::Down || Key == EKeys::Gamepad_DPad_Down)
	{
		InteractionController->MoveSelection(
			FIntPoint(0, 1));

		return FReply::Handled();
	}

	if (Key == EKeys::Left || Key == EKeys::Gamepad_DPad_Left)
	{
		InteractionController->MoveSelection(
			FIntPoint(-1, 0));

		return FReply::Handled();
	}

	if (Key == EKeys::Right || Key == EKeys::Gamepad_DPad_Right)
	{
		InteractionController->MoveSelection(
			FIntPoint(1, 0));

		return FReply::Handled();
	}

	// FaceButton_Bottom won't work here because it will be consumed by the CommonUI.
	if (Key == EKeys::Enter || Key == EKeys::Gamepad_FaceButton_Top)
	{
		InteractionController->HandleConfirm();
		return FReply::Handled();
	}

	if (Key == EKeys::Escape || Key == EKeys::Gamepad_FaceButton_Right)
	{
		InteractionController->HandleCancel();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(
		InGeometry,
		InKeyEvent);
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

	if (!DragOp || !InteractionController ||
		!InteractionController->IsMovingItem()) {
		return false;
	}

	UAuraInventoryComponent* Inventory =
		GetInventoryComponent();

	if (!Inventory) {
		return false;
	}

	UAuraGridInventoryLayout* Layout =
		Cast<UAuraGridInventoryLayout>(
			Inventory->GetLayout());

	if (!Layout) {
		return false;
	}

	const float CellSize =
		Layout->GetCellSize();

	const FVector2D LocalMousePosition =
		InGeometry.AbsoluteToLocal(
			InDragDropEvent.GetScreenSpacePosition());

	const FVector2D ItemTopLeft =
		LocalMousePosition - DragOp->DragOffset;

	const FIntPoint ItemSize =
		InteractionController->GetMoveState().ItemSize;

	const int32 MaxX =
		Layout->GetColumns() - ItemSize.X;

	const int32 MaxY =
		Layout->GetRows() - ItemSize.Y;

	const FIntPoint Cell(
		FMath::Clamp(
			FMath::RoundToInt(ItemTopLeft.X / CellSize),
			0,
			MaxX),
		FMath::Clamp(
			FMath::RoundToInt(ItemTopLeft.Y / CellSize),
			0,
			MaxY));

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

	if (!DragOp || !InteractionController)
		return;

	InteractionController->CancelItemMove();

	Super::NativeOnDragCancelled(
		InDragDropEvent,
		InOperation);
}