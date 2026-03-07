#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindInputActionByTag(
    const FGameplayTag& InputTag) const {
  for (const FAuraInputAction& Action : AbilityInputActions) {
    if (Action.InputTag == InputTag) {
      return Action.InputAction;
    }
  }

  return nullptr;
}
