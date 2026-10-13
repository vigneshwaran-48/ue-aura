
#pragma once

#include "CoreMinimal.h"
#include "AuraItemAction.generated.h"

class UGameplayAbility;
class UAuraItemActionCondition;

USTRUCT(BlueprintType)
struct AURA_API FAuraItemAction
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
    FText DisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
    TSubclassOf<UGameplayAbility> AbilityClass;

    UPROPERTY(
        EditDefaultsOnly,
        Instanced,
        BlueprintReadOnly,
        Category = "Action")
    TObjectPtr<UAuraItemActionCondition> Condition = nullptr;
};
