#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Inventory/AuraItemHandle.h"
#include "UI/AuraUIContext.h"
#include "AuraItemActionContextObject.generated.h"

class UAuraInventoryComponent;

UCLASS(BlueprintType)
class AURA_API UAuraItemActionContextObject : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "Item Action")
    TObjectPtr<UAuraInventoryComponent> Inventory = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Item Action")
    FAuraItemHandle ItemHandle;

    UPROPERTY(BlueprintReadOnly, Category = "Item Action")
    FAuraUIContext UIContext;
};