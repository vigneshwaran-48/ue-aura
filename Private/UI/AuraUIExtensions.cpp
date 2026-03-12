#include "UI/AuraUIExtensions.h"

#include "CommonActivatableWidget.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "UI/AuraPrimaryGameLayout.h"
#include "UI/AuraUISubsystem.h"

UCommonActivatableWidget* UAuraUIExtensions::PushWidgetToLayer(
    UObject* WorldContextObject, FGameplayTag LayerTag,
    TSubclassOf<UCommonActivatableWidget> WidgetClass) {
  if (!WorldContextObject) {
    return nullptr;
  }

  UWorld* World = WorldContextObject->GetWorld();

  if (!World) {
    return nullptr;
  }

  UGameInstance* GameInstance = World->GetGameInstance();

  if (!GameInstance) {
    return nullptr;
  }

  UAuraUISubsystem* UISubsystem =
      GameInstance->GetSubsystem<UAuraUISubsystem>();

  if (!UISubsystem) {
    return nullptr;
  }

  UAuraPrimaryGameLayout* RootLayout = UISubsystem->GetRootLayout();

  if (!RootLayout) {
    return nullptr;
  }

  return RootLayout->PushWidgetToLayer(LayerTag, WidgetClass);
}