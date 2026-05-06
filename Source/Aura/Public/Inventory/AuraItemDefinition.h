#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Inventory/Fragments/AuraItemFragment.h"
#include "AuraItemDefinition.generated.h"

UCLASS(BlueprintType)
class AURA_API UAuraItemDefinition : public UPrimaryDataAsset {
  GENERATED_BODY()

public:
  UPROPERTY(EditDefaultsOnly, Category = "Item")
  FGameplayTag ItemTag;

  UPROPERTY(EditDefaultsOnly, Instanced, Category = "Fragments")
  TArray<TObjectPtr<UAuraItemFragment>> Fragments;

  template <typename T> const T *FindFragment() const {
    for (const UAuraItemFragment *Frag : Fragments) {
      if (const T *Typed = Cast<T>(Frag)) {
        return Typed;
      }
    }
    return nullptr;
  }

#if WITH_EDITOR
  virtual EDataValidationResult
  IsDataValid(FDataValidationContext &Context) const override;
#endif
};
