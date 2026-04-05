#pragma once

#include "AuraOverlayTypes.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AuraOverlayDefinition.generated.h"

UCLASS(BlueprintType)
class AURA_API UAuraOverlayDefinition : public UPrimaryDataAsset {
  GENERATED_BODY()

 public:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  EAuraOverlayType OverlayType;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
  FGameplayTagContainer Slots;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FAuraOverlayStanceSet PoseSet;

#if WITH_EDITOR
  virtual EDataValidationResult IsDataValid(
      FDataValidationContext& Context) const override;
#endif
};