#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AuraAbilitySystemComponent.generated.h"

UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent {
  GENERATED_BODY()

 public:
  void AbilityInputTagPressed(const FGameplayTag& InputTag);
  void AbilityInputTagReleased(const FGameplayTag& InputTag);
};