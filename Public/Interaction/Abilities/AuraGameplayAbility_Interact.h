#pragma once

#include "Abilities/GameplayAbility.h"
#include "CoreMinimal.h"
#include "AuraGameplayAbility_Interact.generated.h"

class UAuraInteractionComponent;

UCLASS()
class AURA_API UAuraGameplayAbility_Interact : public UGameplayAbility {
  GENERATED_BODY()

 public:
  UAuraGameplayAbility_Interact();

 protected:
  virtual void ActivateAbility(
      const FGameplayAbilitySpecHandle Handle,
      const FGameplayAbilityActorInfo* ActorInfo,
      const FGameplayAbilityActivationInfo ActivationInfo,
      const FGameplayEventData* TriggerEventData) override;
};