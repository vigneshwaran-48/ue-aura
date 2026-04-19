#include "Inventory/Abilities/AGA_ToggleInventory.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "UI/AuraUIManagerComponent.h"

UAGA_ToggleInventory::UAGA_ToggleInventory() {
  InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAGA_ToggleInventory::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData) {
  if (!ActorInfo) {
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    return;
  }

  UAuraUIManagerComponent* UIManager =
      Cast<UAuraUIManagerComponent>(ActorInfo->AvatarActor->GetComponentByClass(
          UAuraUIManagerComponent::StaticClass()));
  if (!UIManager) {
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    return;
  }

  UIManager->ToggleUI(TAG_UI_Inventory);

  EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}