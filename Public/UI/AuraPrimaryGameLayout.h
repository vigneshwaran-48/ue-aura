#pragma once

#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "AuraPrimaryGameLayout.generated.h"

class UCommonActivatableWidgetStack;

UCLASS()
class AURA_API UAuraPrimaryGameLayout : public UCommonActivatableWidget {
  GENERATED_BODY()

 public:
  UCommonActivatableWidget* PushWidgetToLayer(
      FGameplayTag LayerTag, TSubclassOf<UCommonActivatableWidget> WidgetClass);

 protected:
  virtual void NativeConstruct() override;

  UPROPERTY(meta = (BindWidget))
  UCommonActivatableWidgetStack* GameStack;

  UPROPERTY(meta = (BindWidget))
  UCommonActivatableWidgetStack* HUDStack;

  UPROPERTY(meta = (BindWidget))
  UCommonActivatableWidgetStack* MenuStack;

  UPROPERTY(meta = (BindWidget))
  UCommonActivatableWidgetStack* ModalStack;

 private:
  TMap<FGameplayTag, UCommonActivatableWidgetStack*> LayerMap;
};