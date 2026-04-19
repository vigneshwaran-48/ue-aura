#include "Inventory/UI/Grid/AuraInventorySlotWidget.h"

#include "Components/SizeBox.h"

void UAuraInventorySlotWidget::SetSlotSize(float InSize) {
  if (RootSizeBox) {
    RootSizeBox->SetWidthOverride(InSize);
    RootSizeBox->SetHeightOverride(InSize);
  }
}
