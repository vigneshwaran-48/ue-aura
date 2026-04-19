#include "Inventory/UI/Grid/AuraInventoryItemWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Inventory/AuraInventoryComponent.h"
#include "Inventory/AuraItemInstance.h"
#include "Inventory/Fragments/AuraItemFragment_Size.h"
#include "Inventory/Layouts/AuraGridInventoryLayout.h"
#include "Inventory/UI/Grid/AuraInventoryDragDropOperation.h"
#include "Inventory/UI/Grid/AuraInventoryGridWidget.h"

void UAuraInventoryItemWidget::InitFromItem(const FAuraItemInstance& Item,
                                            FAuraItemHandle InHandle,
                                            float InCellSize) {
  ItemHandle = InHandle;

  if (const UAuraItemFragment_Size* SizeFrag =
          Item.FindFragment<UAuraItemFragment_Size>()) {
    ItemSize = SizeFrag->Size;
  }

  CellSize = InCellSize;
  CachedItem = &Item;

  if (RootSizeBox) {
    RootSizeBox->SetWidthOverride(ItemSize.X * CellSize);
    RootSizeBox->SetHeightOverride(ItemSize.Y * CellSize);
  }
}

FReply UAuraInventoryItemWidget::NativeOnMouseButtonDown(
    const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
  if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {
    return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this,
                                                        EKeys::LeftMouseButton)
        .NativeReply;
  }

  return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UAuraInventoryItemWidget::NativeOnDragDetected(
    const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
    UDragDropOperation*& OutOperation) {
  UAuraInventoryDragDropOperation* DragOp =
      NewObject<UAuraInventoryDragDropOperation>();

  DragOp->ItemHandle = ItemHandle;
  DragOp->ItemSize = ItemSize;
  DragOp->SourceWidget = this;

  APawn* Pawn = GetOwningPlayerPawn();
  UAuraInventoryComponent* Inventory =
      Pawn ? Pawn->FindComponentByClass<UAuraInventoryComponent>() : nullptr;

  if (Inventory) {
    UAuraGridInventoryLayout* Layout =
        Cast<UAuraGridInventoryLayout>(Inventory->GetLayout());

    if (Layout) {
      FIntPoint Pos;
      if (Layout->GetItemPosition(ItemHandle, Pos)) {
        DragOp->OriginalPosition = Pos;
      }

      // Layout->RemoveItem(ItemHandle);
    }
  }

  UAuraInventoryItemWidget* DragVisual =
      CreateWidget<UAuraInventoryItemWidget>(this, GetClass());

  if (DragVisual) {
    DragVisual->InitFromItem(*CachedItem, ItemHandle, CellSize);

    DragOp->DefaultDragVisual = DragVisual;
  }
  OutOperation = DragOp;

  RemoveFromParent();
}