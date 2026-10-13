
#pragma once

#include "CoreMinimal.h"
#include "Inventory/AuraItemHandle.h"
#include "UI/AuraUIContext.h"
#include "AuraItemActionCondition.generated.h"

class UAuraInventoryComponent;

USTRUCT(BlueprintType)
struct AURA_API FAuraItemActionContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Action")
    TObjectPtr<UAuraInventoryComponent> Inventory = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Action")
    FAuraItemHandle ItemHandle;

    UPROPERTY(BlueprintReadOnly, Category = "Action")
    FAuraUIContext UIContext;
};

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class AURA_API UAuraItemActionCondition : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, Category = "Inventory|Item Action")
    bool Evaluate(const FAuraItemActionContext& Context) const;

    virtual bool Evaluate_Implementation(
        const FAuraItemActionContext& Context) const;
};
