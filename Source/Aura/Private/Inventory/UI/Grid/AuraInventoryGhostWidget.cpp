#include "Inventory/UI/Grid/AuraInventoryGhostWidget.h"

void UAuraInventoryGhostWidget::SetGhostState_Implementation(
    EGhostWidgetState NewState) {
  const FLinearColor TargetColor =
      (NewState == EGhostWidgetState::Valid) ? ValidColor : InvalidColor;
  SetColorAndOpacity(TargetColor);
}
