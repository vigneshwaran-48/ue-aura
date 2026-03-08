#include "AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityInputTagPressed(
    const FGameplayTag& InputTag) {
  if (!InputTag.IsValid()) return;

  // Copying the ability specs before activating those. In interactable ability
  // cases where they will remove themselves from the ability system component
  // after activation, it will cause issue due to removal of a element from a
  // array while iterating over it.
  TArray<FGameplayAbilitySpecHandle> HandlesToActivate;
  for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
    if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) {
      HandlesToActivate.Add(AbilitySpec.Handle);
    }
  }

  for (const FGameplayAbilitySpecHandle& Handle : HandlesToActivate) {
    TryActivateAbility(Handle);
  }
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(
    const FGameplayTag& InputTag) {
  if (!InputTag.IsValid()) return;

  for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
    if (AbilitySpec.Ability &&
        AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) {
      AbilitySpec.InputPressed = false;

      if (AbilitySpec.IsActive()) {
        AbilitySpec.Ability->InputReleased(AbilitySpec.Handle,
                                           AbilityActorInfo.Get(),
                                           AbilitySpec.ActivationInfo);
      }
    }
  }
}