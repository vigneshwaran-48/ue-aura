#pragma once

#include "Animation/Overlay/AuraOverlayDefinition.h"
#include "CoreMinimal.h"
#include "Equipment/AuraEquipmentInstance.h"
#include "AuraWeaponEquipmentInstance.generated.h"

class UAuraEquipmentDefinition;
class UAuraAbilitySystemComponent;

UCLASS()
class AURA_API UAuraWeaponEquipmentInstance : public UAuraEquipmentInstance {
  GENERATED_BODY()

 public:
  virtual void OnEquipped_Implementation(
      UAuraAbilitySystemComponent* ASC) override;

  virtual void OnUnequipped_Implementation(
      UAuraAbilitySystemComponent* ASC) override;

 protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Overlay")
  TObjectPtr<UAuraOverlayDefinition> OverlayDefinition;
};
