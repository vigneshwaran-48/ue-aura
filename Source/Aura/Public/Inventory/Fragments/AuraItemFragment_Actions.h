
#pragma once

#include "Inventory/Actions/AuraItemAction.h"
#include "Inventory/Fragments/AuraItemFragment.h"
#include "AuraItemFragment_Actions.generated.h"

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class AURA_API UAuraItemFragment_Actions : public UAuraItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions")
    TArray<FAuraItemAction> Actions;
};
