#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inventory/Fragments/AuraItemFragment.h"
#include "AuraItemFragment_LayoutBehavior.generated.h"

UCLASS()
class AURA_API UAuraItemFragment_LayoutBehavior : public UAuraItemFragment {
  GENERATED_BODY()

 public:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
  FGameplayTag LayoutBehaviorTag;
};
