
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AuraItemAction.generated.h"

class UGameplayAbility;
class UAuraItemActionCondition;
class UAuraItemActionGameplayAbility;

USTRUCT(BlueprintType)
struct AURA_API FAuraItemAction
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
    FText DisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
    TSubclassOf<UAuraItemActionGameplayAbility> AbilityClass;

    UPROPERTY(
        EditDefaultsOnly,
        Instanced,
        BlueprintReadOnly,
        Category = "Action")
    TObjectPtr<UAuraItemActionCondition> Condition = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    FGameplayTag InputTag;
};
