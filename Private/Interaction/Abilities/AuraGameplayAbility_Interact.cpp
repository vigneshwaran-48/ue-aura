#include "Interaction/Abilities/AuraGameplayAbility_Interact.h"

#include "GameFramework/Actor.h"
#include "Interaction/AuraInteractionComponent.h"

UAuraGameplayAbility_Interact::UAuraGameplayAbility_Interact() {
  InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAuraGameplayAbility_Interact::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData) {
  if (!ActorInfo || !ActorInfo->AvatarActor.IsValid()) {
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    return;
  }

  AActor* AvatarActor = ActorInfo->AvatarActor.Get();

  UAuraInteractionComponent* InteractionComponent =
      AvatarActor->FindComponentByClass<UAuraInteractionComponent>();

  if (!InteractionComponent) {
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    return;
  }

  InteractionComponent->UpdateInteractionTarget();
  InteractionComponent->ExecuteInteraction();

  EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}