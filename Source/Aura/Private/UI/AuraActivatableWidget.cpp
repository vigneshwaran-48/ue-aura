#include "UI/AuraActivatableWidget.h"

#include "UI/AuraUIContextSubsystem.h"

UAuraActivatableWidget::UAuraActivatableWidget(
	const FObjectInitializer& ObjectInitializer) {
}

TOptional<FUIInputConfig> UAuraActivatableWidget::GetDesiredInputConfig()
const {
	FUIInputConfig ConfigOverride;

	switch (InputMode) {
	case EAuraWidgetInputMode::Game:
		ConfigOverride = FUIInputConfig(
			ECommonInputMode::Game,
			GameMouseCaptureMode);
		break;
	case EAuraWidgetInputMode::GameAndMenu:
		ConfigOverride = FUIInputConfig(
			ECommonInputMode::All, GameMouseCaptureMode);
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

UGameViewportClient* UAuraActivatableWidget::GetViewportClient() const {
	UGameViewportClient* ViewportClient{ nullptr };
	if (const APlayerController* PC = GetOwningPlayer<APlayerController>()) {
		if (const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer()) {
			ViewportClient = LocalPlayer->ViewportClient;
		}
	}
	return ViewportClient;
}

bool UAuraActivatableWidget::ConsumeUIContext(
	FGameplayTag UITag,
	FAuraUIContext& OutContext) {
	if (!UITag.IsValid()) {
		return false;
	}

	const APlayerController* PC =
		GetOwningPlayer<APlayerController>();

	if (!PC) {
		return false;
	}

	const ULocalPlayer* LocalPlayer =
		PC->GetLocalPlayer();

	if (!LocalPlayer) {
		return false;
	}

	UAuraUIContextSubsystem* ContextSubsystem =
		LocalPlayer->GetSubsystem<UAuraUIContextSubsystem>();

	if (!ContextSubsystem) {
		return false;
	}

	return ContextSubsystem->ConsumeContext(
		UITag,
		OutContext);
}