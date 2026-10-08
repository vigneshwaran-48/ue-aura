#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "AuraUIContext.h"
#include "AuraUIContextSubsystem.generated.h"

UCLASS()
class AURA_API UAuraUIContextSubsystem : public ULocalPlayerSubsystem
{
    GENERATED_BODY()

public:
    void SetContext(
        FGameplayTag UITag,
        const FAuraUIContext& Context);

    bool ConsumeContext(
        FGameplayTag UITag,
        FAuraUIContext& OutContext);

    bool HasContext(FGameplayTag UITag) const;

    void ClearContext(FGameplayTag UITag);

private:
    UPROPERTY()
    TMap<FGameplayTag, FAuraUIContext> Contexts;
};