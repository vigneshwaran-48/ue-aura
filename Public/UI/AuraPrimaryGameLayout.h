#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/AuraActivatableWidget.h"
#include "AuraPrimaryGameLayout.generated.h"

class UAuraTaggedWidgetStack;

UCLASS()
class AURA_API UAuraPrimaryGameLayout : public UAuraActivatableWidget {
  GENERATED_BODY()

 public:
  UCommonActivatableWidget* PushWidgetToLayer(
      FGameplayTag LayerTag, TSubclassOf<UCommonActivatableWidget> WidgetClass);

 protected:
  virtual void NativeConstruct() override;

 private:
  UPROPERTY()
  TMap<FGameplayTag, UAuraTaggedWidgetStack*> LayerMap;
};