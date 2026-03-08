#include "Interaction/Abilities/AuraGameplayAbility_Interactable.h"

#include "AbilitySystemComponent.h"
#include "Interaction/AuraInteractionComponent.h"

UAuraGameplayAbility_Interactable::UAuraGameplayAbility_Interactable() {
  InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

  ActivationOwnedTags.AddTag(
      FGameplayTag::RequestGameplayTag("Ability.Interaction"));

  BlockAbilitiesWithTag.AddTag(
      FGameplayTag::RequestGameplayTag("Ability.Interaction"));
}

void UAuraGameplayAbility_Interactable::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility, bool bWasCancelled) {
  Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
                    bWasCancelled);
}

AActor* UAuraGameplayAbility_Interactable::GetInteractableActor() const {
  if (!CurrentActorInfo || !CurrentActorInfo->AvatarActor.IsValid())
    return nullptr;

  AActor* Avatar = CurrentActorInfo->AvatarActor.Get();

  UAuraInteractionComponent* Interaction =
      Avatar->FindComponentByClass<UAuraInteractionComponent>();

  if (!Interaction) return nullptr;

  return Interaction->GetInteractableActor();
}