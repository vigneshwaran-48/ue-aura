#pragma once

#include "Abilities/GameplayAbility.h"
#include "CoreMinimal.h"
#include "Equipment/AuraEquipmentManagerComponent.h"
#include "AuraAbility_UnequipBase.generated.h"

class UAuraEquipmentManagerComponent;

UCLASS()
class AURA_API UAuraAbility_UnequipBase : public UGameplayAbility {
  GENERATED_BODY()

 public:
  UAuraAbility_UnequipBase();

 protected:
  UPROPERTY(EditDefaultsOnly, Category = "Equipment")
  EAuraEquipmentType EquipmentType;

 protected:
  virtual void ActivateAbility(
      const FGameplayAbilitySpecHandle Handle,
      const FGameplayAbilityActorInfo* ActorInfo,
      const FGameplayAbilityActivationInfo ActivationInfo,
      const FGameplayEventData* TriggerEventData) override;

 private:
  UAuraEquipmentManagerComponent* GetEquipmentComponent(
      const FGameplayAbilityActorInfo* ActorInfo) const;
};