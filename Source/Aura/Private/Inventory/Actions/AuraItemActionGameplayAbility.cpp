#include "Inventory/Actions/AuraItemActionGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Inventory/Actions/AuraItemActionContextObject.h"

UAuraItemActionContextObject*
UAuraItemActionGameplayAbility::GetItemActionContext() const
{
    return Cast<UAuraItemActionContextObject>(
        GetCurrentSourceObject());
}

void UAuraItemActionGameplayAbility::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    UAbilitySystemComponent* ASC =
        GetAbilitySystemComponentFromActorInfo();

    Super::EndAbility(
        Handle,
        ActorInfo,
        ActivationInfo,
        bReplicateEndAbility,
        bWasCancelled);

    if (ASC && Handle.IsValid())
        ASC->ClearAbility(Handle);
}