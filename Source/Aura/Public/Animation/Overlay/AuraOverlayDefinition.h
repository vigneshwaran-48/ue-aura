#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AuraOverlayDefinition.generated.h"

UCLASS(BlueprintType)
class AURA_API UAuraOverlayDefinition : public UPrimaryDataAsset {
  GENERATED_BODY()

 public:
  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<UAnimInstance> OverlayAnimBP;
};