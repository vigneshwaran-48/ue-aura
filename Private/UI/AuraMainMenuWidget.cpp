#include "UI/AuraMainMenuWidget.h"

#include "AuraGameplayTags.h"
#include "CommonUIExtensions.h"
#include "Input/CommonUIInputTypes.h"

UAuraMainMenuWidget::UAuraMainMenuWidget(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
  bAutoActivate = true;

  // Change to Menu input mode when this widget is activated
  bSupportsActivationFocus = true;
  InputMode = EAuraWidgetInputMode::Menu;
}

void UAuraMainMenuWidget::NativeConstruct() {
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

void UAuraMainMenuWidget::NativeDestruct() {
  if (MenuActionHandle.IsValid()) {
    MenuActionHandle.Unregister();
  }

  Super::NativeDestruct();
}

void UAuraMainMenuWidget::NativeOnActivated() {
  Super::NativeOnActivated();

  UE_LOG(LogTemp, Log,
         TEXT("Pushing Empty widgets to lower layers to effectively disable "
              "their UI"));

  BlankGameWidget = UCommonUIExtensions::PushContentToLayer_ForPlayer(
      GetOwningLocalPlayer(), TAG_UI_Layer_Game, BlankWidgetClass);
  BlankGameMenuWidget = UCommonUIExtensions::PushContentToLayer_ForPlayer(
      GetOwningLocalPlayer(), TAG_UI_Layer_GameMenu, BlankWidgetClass);
}

void UAuraMainMenuWidget::NativeOnDeactivated() {
  UE_LOG(LogTemp, Log,
         TEXT("Popping Empty widgets to lower layers to effectively re-enable "
              "their UI"));

  if (BlankGameWidget.IsValid()) {
    UCommonUIExtensions::PopContentFromLayer(BlankGameWidget.Get());
    BlankGameWidget.Reset();
  }

  if (BlankGameMenuWidget.IsValid()) {
    UCommonUIExtensions::PopContentFromLayer(BlankGameMenuWidget.Get());
    BlankGameMenuWidget.Reset();
  }

  Super::NativeOnDeactivated();
}

void UAuraMainMenuWidget::HandleMainMenuAction() {
  // When the player gives a "main menu" action while the main menu is open,
  // it means we need to close the main menu and return to game.
  DeactivateWidget();
}
