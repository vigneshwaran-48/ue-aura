#include "Equipment/Weapon/AuraWeaponEquipmentInstance.h"

#include "Animation/Overlay/AuraOverlayComponent.h"
#include "Animation/Overlay/AuraOverlayDefinition.h"
#include "AuraAbilitySystemComponent.h"

void UAuraWeaponEquipmentInstance::OnEquipped_Implementation(
    UAuraAbilitySystemComponent* ASC) {
  Super::OnEquipped_Implementation(ASC);

  if (OwningActor == nullptr) {
    UE_LOG(LogTemp, Error, TEXT("OwningActor is null"));
    return;
  }

  UAuraOverlayComponent* OverlayComp = Cast<UAuraOverlayComponent>(
      OwningActor->GetComponentByClass(UAuraOverlayComponent::StaticClass()));

  if (OverlayComp == nullptr) {
    UE_LOG(LogTemp, Error, TEXT("OverlayComp is null on %s"),
           *GetNameSafe(OwningActor));
    return;
  }

  OverlayComp->SetPoseOverlay(OverlayDefinition);
}

void UAuraWeaponEquipmentInstance::OnUnequipped_Implementation(
    UAuraAbilitySystemComponent* ASC) {
  Super::OnUnequipped_Implementation(ASC);

  if (OwningActor == nullptr) {
    UE_LOG(LogTemp, Error, TEXT("OwningActor is null"));
    return;
  }

  UAuraOverlayComponent* OverlayComp = Cast<UAuraOverlayComponent>(
      OwningActor->GetComponentByClass(UAuraOverlayComponent::StaticClass()));

  if (OverlayComp == nullptr) {
    UE_LOG(LogTemp, Error, TEXT("OverlayComp is null on %s"),
           *GetNameSafe(OwningActor));
    return;
  }

  OverlayComp->ClearPoseOverlay();
}
