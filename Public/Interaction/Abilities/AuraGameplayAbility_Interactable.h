#pragma once

#include "Abilities/GameplayAbility.h"
#include "CoreMinimal.h"
#include "AuraGameplayAbility_Interactable.generated.h"

class UAuraInteractionComponent;

UCLASS(Abstract)
class AURA_API UAuraGameplayAbility_Interactable : public UGameplayAbility {
  GENERATED_BODY()

 public:
  UAuraGameplayAbility_Interactable();

 protected:
  virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
                          const FGameplayAbilityActorInfo* ActorInfo,
                          const FGameplayAbilityActivationInfo ActivationInfo,
                          bool bReplicateEndAbility,
                          bool bWasCancelled) override;

 public:
  UFUNCTION(BlueprintCallable, Category = "Interaction")
  AActor* GetInteractableActor() const;
};