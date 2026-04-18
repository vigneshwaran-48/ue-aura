#pragma once

#include "Inventory/Fragments/AuraItemFragment.h"
#include "CoreMinimal.h"
#include "AuraItemFragment_Size.generated.h"

UCLASS(BlueprintType, EditInlineNew)
class AURA_API UAuraItemFragment_Size : public UAuraItemFragment {
  GENERATED_BODY()

 public:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
  FIntPoint Size = FIntPoint(1, 1);
};