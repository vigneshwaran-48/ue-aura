#pragma once

#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
#include "CoreMinimal.h"
#include "AuraAnimInstance.generated.h"

class UAbilitySystemComponent;

UCLASS()
class AURA_API UAuraAnimInstance : public UAnimInstance {
  GENERATED_BODY()

 public:
  UAuraAnimInstance(const FObjectInitializer& ObjectInitializer);

  virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);

 protected:
  virtual void NativeInitializeAnimation() override;

 protected:
  UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
  FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
};
