#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "AuraInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FAuraInputAction {
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  const UInputAction* InputAction = nullptr;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
            meta = (Categories = "InputTag"))
  FGameplayTag InputTag;
};

UCLASS(BlueprintType)
class AURA_API UAuraInputConfig : public UDataAsset {
  GENERATED_BODY()

 public:
  const UInputAction* FindInputActionByTag(const FGameplayTag& InputTag) const;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TArray<FAuraInputAction> AbilityInputActions;
};