#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AGA_OpenInventory.generated.h"

UCLASS()
class AURA_API UAGA_OpenInventory : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UAGA_OpenInventory();

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;
};