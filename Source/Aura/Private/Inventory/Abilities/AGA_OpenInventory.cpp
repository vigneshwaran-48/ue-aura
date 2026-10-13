#include "Inventory/Abilities/AGA_OpenInventory.h"

#include "AuraGameplayTags.h"
#include "GameFramework/Actor.h"
#include "UI/AuraUIManagerComponent.h"

UAGA_OpenInventory::UAGA_OpenInventory()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAGA_OpenInventory::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    UAuraUIManagerComponent* UIManager =
        Cast<UAuraUIManagerComponent>(
            ActorInfo->AvatarActor->GetComponentByClass(
                UAuraUIManagerComponent::StaticClass()));

    if (!UIManager)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    UIManager->EnableUI(TAG_UI_Inventory);

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}