#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraItemActionGameplayAbility.generated.h"

class UAuraItemActionContextObject;

UCLASS(Abstract)
class AURA_API UAuraItemActionGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UAuraItemActionContextObject* GetItemActionContext() const;

protected:
    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled) override;
};