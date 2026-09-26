#include "Inventory/UI/Grid/AuraInventoryItemWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Inventory/AuraItemInstance.h"
#include "Inventory/Fragments/AuraItemFragment_Size.h"
#include "Inventory/UI/Grid/AuraInventoryDragDropOperation.h"
#include "Inventory/UI/Grid/AuraInventoryGridWidget.h"
#include "Inventory/UI/Grid/AuraInventoryInteractController.h"

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
    const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent,
    UDragDropOperation*& OutOperation)
{
    if (!OwningGrid)
        return;

    UAuraInventoryInteractController* Controller =
        OwningGrid->GetInteractionController();

    if (!Controller)
        return;

    if (!Controller->BeginItemMove(ItemHandle))
        return;

    UAuraInventoryDragDropOperation* DragOp =
        NewObject<UAuraInventoryDragDropOperation>();

    if (!DragOp) {
        Controller->CancelItemMove();
        return;
    }

    const FVector2D LocalMousePos =
        InGeometry.AbsoluteToLocal(
            InMouseEvent.GetScreenSpacePosition());

    DragOp->ItemHandle = ItemHandle;
    DragOp->DragOffset = LocalMousePos;
    DragOp->InteractionController = Controller;

    // Create drag visual here for now.

    OutOperation = DragOp;
}

void UAuraInventoryItemWidget::SetSelected(bool bSelected) {
    UE_LOG(
        LogTemp,
        Warning,
        TEXT("ItemWidget %p SetSelected(%s) Highlight=%p Visibility=%d"),
        this,
        bSelected ? TEXT("true") : TEXT("false"),
        SelectionHighlight.Get(),
        SelectionHighlight
        ? static_cast<int32>(SelectionHighlight->GetVisibility())
        : -1);

    if (!SelectionHighlight) {
        return;
    }

    SelectionHighlight->SetVisibility(
        bSelected
        ? ESlateVisibility::HitTestInvisible
        : ESlateVisibility::Collapsed);
}