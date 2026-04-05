#include "Animation/Overlay/AuraOverlayComponent.h"

void UAuraOverlayComponent::SetBaseOverlay(
    const UAuraOverlayDefinition* NewOverlay) {
  BaseOverlay = NewOverlay;
}

void UAuraOverlayComponent::SetStatusOverlay(
    const UAuraOverlayDefinition* NewOverlay) {
  StatusOverlay = NewOverlay;
}

void UAuraOverlayComponent::AddEquipmentOverlay(
    const UAuraOverlayDefinition* NewOverlay) {
  if (!NewOverlay) return;

  // Remove conflicting overlays
  for (const FGameplayTag& NewSlot : NewOverlay->Slots) {
    EquipmentOverlays.RemoveAll([&](const UAuraOverlayDefinition* Existing) {
      return DoesOverlayUseSlot(Existing, NewSlot);
    });
  }

  EquipmentOverlays.Add(NewOverlay);
}

void UAuraOverlayComponent::RemoveEquipmentOverlayBySlot(FGameplayTag SlotTag) {
  EquipmentOverlays.RemoveAll([&](const UAuraOverlayDefinition* Existing) {
    return DoesOverlayUseSlot(Existing, SlotTag);
  });
}

bool UAuraOverlayComponent::DoesOverlayUseSlot(
    const UAuraOverlayDefinition* Overlay, const FGameplayTag& SlotTag) const {
  if (!Overlay) return false;

  for (const FGameplayTag& Slot : Overlay->Slots) {
    if (Slot == SlotTag) {
      return true;
    }
  }

  return false;
}