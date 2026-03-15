#include "UI/AuraHUDLayout.h"

#include "AuraGameplayTags.h"
#include "CommonUIExtensions.h"
#include "Input/CommonUIInputTypes.h"

UAuraHUDLayout::UAuraHUDLayout(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
  bAutoActivate = true;

  // Change to GameAndMenu input mode when this widget is activated
  bSupportsActivationFocus = true;
  InputMode = EAuraWidgetInputMode::GameAndMenu;
}

void UAuraHUDLayout::NativeConstruct() {
  Super::NativeConstruct();

  if (not IsDesignTime()) {
    // This registers a "UI Action Binding" with CommonUI, such that when the
    // UI_Action_MainMenu UI Action occurs, the delegate is called which
    // forwards the event to our own HandleMainMenuAction
    const auto ActionTag = FUIActionTag::ConvertChecked(TAG_UI_Action_MainMenu);
    const auto Delegate =
        FSimpleDelegate::CreateUObject(this, &ThisClass::HandleMainMenuAction);
    constexpr bool bShouldDisplayInActionBar = false;
    MenuActionHandle = RegisterUIActionBinding(
        FBindUIActionArgs(ActionTag, bShouldDisplayInActionBar, Delegate));
  }
}

void UAuraHUDLayout::NativeDestruct() {
  if (MenuActionHandle.IsValid()) {
    MenuActionHandle.Unregister();
  }

  Super::NativeDestruct();
}

void UAuraHUDLayout::HandleMainMenuAction() {
  if (ensureMsgf(not MainMenuClass.IsNull(),
                 TEXT("%hs: You must configure MainMenuClass"), __FUNCTION__)) {
    UCommonUIExtensions::PushStreamedContentToLayer_ForPlayer(
        GetOwningLocalPlayer(), TAG_UI_Layer_Menu, MainMenuClass);
  }
}
