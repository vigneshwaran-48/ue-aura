#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "Input/AuraInputConfig.h"
#include "AuraEnhancedInputComponent.generated.h"

class UAuraInputConfig;

UCLASS()
class AURA_API UAuraEnhancedInputComponent : public UEnhancedInputComponent {
  GENERATED_BODY()

 public:
  template <class UserClass, typename PressedFuncType,
            typename ReleasedFuncType>
  void BindAbilityActions(const UAuraInputConfig* InputConfig,
                          UserClass* Object, PressedFuncType PressedFunc,
                          ReleasedFuncType ReleasedFunc) {
    check(InputConfig);

    for (const FAuraInputAction& Action : InputConfig->AbilityInputActions) {
      if (!Action.InputAction || !Action.InputTag.IsValid()) {
        continue;
      }

      if (PressedFunc) {
        BindAction(Action.InputAction, ETriggerEvent::Started, Object,
                   PressedFunc, Action.InputTag);
      }

      if (ReleasedFunc) {
        BindAction(Action.InputAction, ETriggerEvent::Completed, Object,
                   ReleasedFunc, Action.InputTag);
      }
    }
  }
};