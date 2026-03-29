#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AuraAbilitySystemComponent.generated.h"

UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent {
  GENERATED_BODY()

 public:
  virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
  virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

  virtual void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
  virtual void ClearAbilityInput();

  void AbilityInputTagPressed(const FGameplayTag& InputTag);
  void AbilityInputTagReleased(const FGameplayTag& InputTag);

  UFUNCTION(BlueprintCallable)
  void AddGameplayTag(const FGameplayTag& GameplayTag);

  UFUNCTION(BlueprintCallable)
  void RemoveGameplayTag(const FGameplayTag& GameplayTag);

 private:
  // Handles to abilities that had their input pressed this frame
  TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

  // Handles to abilities that had their input released this frame
  TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

  // Handles to abilities with input currently held down
  TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};