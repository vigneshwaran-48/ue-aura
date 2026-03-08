#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "AuraInteractionOption.generated.h"

class UGameplayAbility;
class UAuraGameplayAbility_Interactable;

USTRUCT(BlueprintType)
struct FAuraInteractionOption {
  GENERATED_BODY()

 public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FText DisplayName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TSubclassOf<UAuraGameplayAbility_Interactable> AbilityClass;
};