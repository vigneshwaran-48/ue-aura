#pragma once

#include "CoreMinimal.h"
#include "Inventory/Fragments/AuraItemFragment.h"
#include "AuraItemFragment_EquippableItem.generated.h"

class UAuraEquipmentDefinition;

UCLASS()
class AURA_API UAuraItemFragment_EquippableItem : public UAuraItemFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	TObjectPtr<UAuraEquipmentDefinition> EquipmentDefinition;
};
