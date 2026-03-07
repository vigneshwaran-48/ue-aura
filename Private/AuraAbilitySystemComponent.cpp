#include "AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityInputTagPressed(
    const FGameplayTag& InputTag) {
  if (!InputTag.IsValid()) return;

  for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
    if (AbilitySpec.Ability &&
        AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) {
      AbilitySpec.InputPressed = true;

      if (AbilitySpec.IsActive()) {
        AbilitySpec.Ability->InputPressed(AbilitySpec.Handle,
                                          AbilityActorInfo.Get(),
                                          AbilitySpec.ActivationInfo);
      } else {
        TryActivateAbility(AbilitySpec.Handle);
      }
    }
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