#pragma once

#include "CoreMinimal.h"
#include "Inventory/Actions/AuraItemActionGameplayAbility.h"
#include "AuraAbility_EquipFromInventory.generated.h"

UCLASS()
class AURA_API UAuraAbility_EquipFromInventory : public UAuraItemActionGameplayAbility
{
	GENERATED_BODY()
	
protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;
};
