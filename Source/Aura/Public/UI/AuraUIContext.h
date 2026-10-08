#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AuraUIContext.generated.h"

USTRUCT(BlueprintType)
struct AURA_API FAuraUIContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FGameplayTag ContextTag;

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UObject> ContextObject = nullptr;
};