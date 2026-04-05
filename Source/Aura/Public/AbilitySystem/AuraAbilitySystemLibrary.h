#pragma once

#include "AuraAbilitySystemComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAuraAbilitySystemComponent;

UCLASS()
class AURA_API UAuraAbilitySystemLibrary
    : public UBlueprintFunctionLibrary {
  GENERATED_BODY()

 public:
  UFUNCTION(BlueprintCallable, Category = "Aura|AbilitySystem",
            meta = (DefaultToSelf = "Actor"))
  static UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent(
      AActor* Actor);
};