#pragma once

#include "Interaction/Abilities/AuraGameplayAbility_Interactable.h"
#include "CoreMinimal.h"
#include "AuraGameplayAbility_InteractHold.generated.h"

UCLASS()
class AURA_API UAuraGameplayAbility_InteractHold
    : public UAuraGameplayAbility_Interactable {
  GENERATED_BODY()

 public:
  UAuraGameplayAbility_InteractHold();

 protected:
  virtual void ActivateAbility(
      const FGameplayAbilitySpecHandle Handle,
      const FGameplayAbilityActorInfo* ActorInfo,
      const FGameplayAbilityActivationInfo ActivationInfo,
      const FGameplayEventData* TriggerEventData) override;

  virtual void InputReleased(
      const FGameplayAbilitySpecHandle Handle,
      const FGameplayAbilityActorInfo* ActorInfo,
      const FGameplayAbilityActivationInfo ActivationInfo) override;

  UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
  void OnHoldInputCompleted();

 protected:
  UPROPERTY(EditDefaultsOnly, Category = "Interaction")
  float HoldTime = 1.5f;

  UFUNCTION()
  void OnHoldCompleted();

 private:
  bool bHoldCompleted = false;
};