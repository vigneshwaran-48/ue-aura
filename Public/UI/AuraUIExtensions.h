#pragma once

#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraUIExtensions.generated.h"

class UCommonActivatableWidget;

UCLASS()
class AURA_API UAuraUIExtensions : public UBlueprintFunctionLibrary {
  GENERATED_BODY()

 public:
  UFUNCTION(BlueprintCallable, Category = "AuraUI",
            meta = (WorldContext = "WorldContextObject"))
  static UCommonActivatableWidget* PushWidgetToLayer(
      UObject* WorldContextObject, FGameplayTag LayerTag,
      TSubclassOf<UCommonActivatableWidget> WidgetClass);
};