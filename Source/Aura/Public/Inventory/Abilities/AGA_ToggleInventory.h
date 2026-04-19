#pragma once

#include "Abilities/GameplayAbility.h"
#include "CoreMinimal.h"
#include "AGA_ToggleInventory.generated.h"

UCLASS()
class AURA_API UAGA_ToggleInventory : public UGameplayAbility {
  GENERATED_BODY()

 public:
  UAGA_ToggleInventory();

 protected:
  virtual void ActivateAbility(
      const FGameplayAbilitySpecHandle Handle,
      const FGameplayAbilityActorInfo* ActorInfo,
      const FGameplayAbilityActivationInfo ActivationInfo,
      const FGameplayEventData* TriggerEventData) override;
};