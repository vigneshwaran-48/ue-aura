#include "UI/AuraActivatableWidget.h"

UAuraActivatableWidget::UAuraActivatableWidget(
    const FObjectInitializer& ObjectInitializer) {
  bAutoActivate = true;

  bSetVisibilityOnActivated = true;
  bSetVisibilityOnDeactivated = true;
  ActivatedVisibility = ESlateVisibility::SelfHitTestInvisible;
  DeactivatedVisibility = ESlateVisibility::Collapsed;

  bSupportsActivationFocus = false;  // By default, DO NOT change input settings
                                     // on each activation/deactivation
}

TOptional<FUIInputConfig> UAuraActivatableWidget::GetDesiredInputConfig()
    const {
  FUIInputConfig ConfigOverride;

  switch (InputMode) {
    case EAuraWidgetInputMode::Game:
      ConfigOverride = FUIInputConfig(
          ECommonInputMode::Game,
          EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
      break;
    case EAuraWidgetInputMode::GameAndMenu:
      ConfigOverride = FUIInputConfig(
          ECommonInputMode::All, EMouseCaptureMode::CaptureDuringMouseDown);
      break;
    case EAuraWidgetInputMode::Menu:
      ConfigOverride =
          FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
      break;
    case EAuraWidgetInputMode::Default:
      // By default, no input change is desired, return an empty config
      return TOptional<FUIInputConfig>();
    default:
      checkf(false, TEXT("Unexpected InputMode value"));
  }

  return ConfigOverride;
}

void UAuraActivatableWidget::NativePreConstruct() {
  Super::NativePreConstruct();

  if (not IsDesignTime()) {
    // If activatable widgets are supposed to be a specific visibility when not
    // active, set that Visibility now
    if (not bAutoActivate && not IsActivated() &&
        GetVisibility() != DeactivatedVisibility) {
      SetVisibility(DeactivatedVisibility);
    }
  }
}

void UAuraActivatableWidget::NativeConstruct() {
  UE_LOG(LogTemp, Log,
         TEXT("%hs: %s: Widget constructing (DesignTime?=%hi)"), __FUNCTION__,
         *GetClass()->GetName(), IsDesignTime());
  Super::NativeConstruct();
}

void UAuraActivatableWidget::NativeDestruct() {
  UE_LOG(LogTemp, Log,
         TEXT("%hs: %s: Widget destructing (DesignTime?=%hi)"), __FUNCTION__,
         *GetClass()->GetName(), IsDesignTime());

  Super::NativeDestruct();
}

void UAuraActivatableWidget::NativeOnActivated() {
  UE_LOG(LogTemp, Log, TEXT("%hs: %s: Widget Activating"), __FUNCTION__,
         *GetClass()->GetName());

  Super::NativeOnActivated();
}

void UAuraActivatableWidget::NativeOnDeactivated() {
  UE_LOG(LogTemp, Log, TEXT("%hs: %s: Widget Deactivating"), __FUNCTION__,
         *GetClass()->GetName());

  Super::NativeOnDeactivated();
}

UGameViewportClient* UAuraActivatableWidget::GetViewportClient() const {
  UGameViewportClient* ViewportClient{nullptr};
  if (const APlayerController* PC = GetOwningPlayer<APlayerController>()) {
    if (const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer()) {
      ViewportClient = LocalPlayer->ViewportClient;
    }
  }
  return ViewportClient;
}
