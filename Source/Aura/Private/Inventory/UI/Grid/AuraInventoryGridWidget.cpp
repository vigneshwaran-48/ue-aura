#include "Inventory/UI/Grid/AuraInventoryGridWidget.h"

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
#include "Inventory/UI/Grid/AuraInventoryItemWidget.h"
#include "Inventory/UI/Grid/AuraInventorySlotWidget.h"

void UAuraInventoryGridWidget::NativeConstruct() {
  Super::NativeConstruct();

  BuildGrid();
  PopulateItems();
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
    }
  }

  UE_LOG(LogTemp, Warning, TEXT("Grid built %dx%d CellSize %.1f"), Rows, Cols,
         CellSize);
}

void UAuraInventoryGridWidget::PopulateItems() {
  if (!ItemCanvas || !ItemWidgetClass) {
    return;
  }

  ItemCanvas->ClearChildren();

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

    const UAuraItemFragment_Size* SizeFragment =
        Item->FindFragment<UAuraItemFragment_Size>();

    const FIntPoint Size = SizeFragment ? SizeFragment->Size : FIntPoint(1, 1);

    UAuraInventoryItemWidget* ItemWidget =
        CreateWidget<UAuraInventoryItemWidget>(this, ItemWidgetClass);

    if (!ItemWidget) {
      continue;
    }

    ItemWidget->InitFromItem(*Item, Handle, Layout->GetCellSize());

    UCanvasPanelSlot* CanvasSlot = ItemCanvas->AddChildToCanvas(ItemWidget);

    if (!CanvasSlot) {
      continue;
    }

    CanvasSlot->SetAutoSize(false);

    CanvasSlot->SetPosition(
        FVector2D(Position.X * CellSize, Position.Y * CellSize));

    CanvasSlot->SetSize(FVector2D(Size.X * CellSize, Size.Y * CellSize));
  }
}

bool UAuraInventoryGridWidget::NativeOnDrop(
    const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation) {
  UAuraInventoryDragDropOperation* DragOp =
      Cast<UAuraInventoryDragDropOperation>(InOperation);

  if (!DragOp) {
    return false;
  }

  UAuraInventoryComponent* Inventory = GetInventoryComponent();
  if (!Inventory) {
    return false;
  }

  UAuraGridInventoryLayout* Layout =
      Cast<UAuraGridInventoryLayout>(Inventory->GetLayout());

  if (!Layout) {
    return false;
  }

  const float CellSize = Layout->GetCellSize();

  FVector2D LocalPos =
      InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());

  FIntPoint DropCell(FMath::FloorToInt(LocalPos.X / CellSize),
                     FMath::FloorToInt(LocalPos.Y / CellSize));

  bool bPlaced = Layout->TryAddItemAt(DragOp->ItemHandle, DropCell);

  if (!bPlaced) {
    Layout->TryAddItemAt(DragOp->ItemHandle, DragOp->OriginalPosition);
  }

  PopulateItems();

  return true;
}