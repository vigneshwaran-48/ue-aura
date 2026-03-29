#include "AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityInputTagPressed(
    const FGameplayTag& InputTag) {
  if (!InputTag.IsValid()) return;

  for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items) {
    if (AbilitySpec.Ability &&
        AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)) {
      // Track that this ability's input was pressed
      InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
      InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);

      UE_LOG(LogTemp, Log, TEXT("AbilityInputTagPressed added: %s "),
             *InputTag.ToString());
    }
  }
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(
    const FGameplayTag& InputTag) {
  if (!InputTag.IsValid()) return;

  for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items) {
    if (AbilitySpec.Ability &&
        AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)) {
      // Track that this ability's input was released
      InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
      InputHeldSpecHandles.Remove(AbilitySpec.Handle);

      UE_LOG(LogTemp, Log, TEXT("AbilityInputTagReleased added: %s "),
             *InputTag.ToString());
    }
  }
}

void UAuraAbilitySystemComponent::AddGameplayTag(
    const FGameplayTag& GameplayTag) {
  if (!HasMatchingGameplayTag(GameplayTag)) {
    AddLooseGameplayTag(GameplayTag);
  }
}

void UAuraAbilitySystemComponent::RemoveGameplayTag(
    const FGameplayTag& GameplayTag) {
  RemoveLooseGameplayTag(GameplayTag);
}

void UAuraAbilitySystemComponent::AbilitySpecInputPressed(
    FGameplayAbilitySpec& Spec) {
  Super::AbilitySpecInputPressed(Spec);

  // Mark the spec as input pressed
  if (Spec.IsActive()) {
    // If the ability is already active, notify it
    // Use the ability instance's activation info for instanced abilities
    if (UGameplayAbility* InstancedAbility = Spec.GetPrimaryInstance()) {
      UE_LOG(LogTemp, Log, TEXT("Input tag pressed notified"));
      InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed,
                            Spec.Handle,
                            InstancedAbility->GetCurrentActivationInfo()
                                .GetActivationPredictionKey());
    }
  }
}

void UAuraAbilitySystemComponent::AbilitySpecInputReleased(
    FGameplayAbilitySpec& Spec) {
  Super::AbilitySpecInputReleased(Spec);

  // Mark the spec as input released
  if (Spec.IsActive()) {
    // If the ability is already active, notify it
    if (UGameplayAbility* InstancedAbility = Spec.GetPrimaryInstance()) {
      UE_LOG(LogTemp, Log, TEXT("Input tag released notified"));
      InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased,
                            Spec.Handle,
                            InstancedAbility->GetCurrentActivationInfo()
                                .GetActivationPredictionKey());
    }
  }
}

void UAuraAbilitySystemComponent::ProcessAbilityInput(float DeltaTime,
                                                      bool bGamePaused) {
  // Process all abilities that had input pressed this frame
  TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

  for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles) {
    if (FGameplayAbilitySpec* AbilitySpec =
            FindAbilitySpecFromHandle(SpecHandle)) {
      if (AbilitySpec->Ability) {
        AbilitySpec->InputPressed = true;

        if (AbilitySpec->IsActive()) {
          // Ability is already active, pass along the input event
          AbilitySpecInputPressed(*AbilitySpec);
        } else {
          // Ability is not active, try to activate it
          AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
        }
      }
    }
  }

  // Attempt to activate all abilities that had input pressed
  for (const FGameplayAbilitySpecHandle& AbilitySpecHandle :
       AbilitiesToActivate) {
    TryActivateAbility(AbilitySpecHandle);
  }

  // Process all abilities that had input released this frame
  for (const FGameplayAbilitySpecHandle& SpecHandle :
       InputReleasedSpecHandles) {
    if (FGameplayAbilitySpec* AbilitySpec =
            FindAbilitySpecFromHandle(SpecHandle)) {
      if (AbilitySpec->Ability) {
        AbilitySpec->InputPressed = false;

        if (AbilitySpec->IsActive()) {
          // Ability is active, notify it of input release
          AbilitySpecInputReleased(*AbilitySpec);
        } else {
          UE_LOG(LogTemp, Log,
                 TEXT("Ability spec is not active to release input"));
        }
      }
    }
  }

  // Clear the pressed and released lists for next frame
  InputPressedSpecHandles.Reset();
  InputReleasedSpecHandles.Reset();
}

void UAuraAbilitySystemComponent::ClearAbilityInput() {
  InputPressedSpecHandles.Reset();
  InputReleasedSpecHandles.Reset();
  InputHeldSpecHandles.Reset();
}
