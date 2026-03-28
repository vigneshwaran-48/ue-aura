#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AuraPawnData.generated.h"

class UInputMappingContext;
class UAuraInputConfig;
class UAuraAbilitySet;
class UUserWidget;
class UAnimInstance;
class UCommonActivatableWidget;

UCLASS(BlueprintType)
class AURA_API UAuraPawnData : public UPrimaryDataAsset {
  GENERATED_BODY()

 public:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
  TObjectPtr<UInputMappingContext> InputMappingContext;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
  TObjectPtr<UAuraInputConfig> InputConfig;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
  TArray<TObjectPtr<UAuraAbilitySet>> AbilitySets;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
  TSubclassOf<UCommonActivatableWidget> HUDLayoutClass;
};
