#include "Interaction/Abilities/AuraGameplayAbility_InteractHold.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

UAuraGameplayAbility_InteractHold::UAuraGameplayAbility_InteractHold() {
  InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAuraGameplayAbility_InteractHold::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData) {
  Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
  UE_LOG(LogTemp, Log, TEXT("Start Hold Interact Ability timer!"));
  bHoldCompleted = false;

  UAbilityTask_WaitDelay* DelayTask =
      UAbilityTask_WaitDelay::WaitDelay(this, HoldTime);
  DelayTask->OnFinish.AddUniqueDynamic(
      this, &UAuraGameplayAbility_InteractHold::OnHoldCompleted);

  DelayTask->ReadyForActivation();
}

void UAuraGameplayAbility_InteractHold::InputReleased(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo) {
  Super::InputPressed(Handle, ActorInfo, ActivationInfo);
  UE_LOG(LogTemp, Log, TEXT("Input released!"));
  if (!bHoldCompleted) {
    EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
  }
}

void UAuraGameplayAbility_InteractHold::OnHoldCompleted() {
  bHoldCompleted = true;
  UE_LOG(LogTemp, Log, TEXT("Hold completed!"));
  OnHoldInputCompleted();
}