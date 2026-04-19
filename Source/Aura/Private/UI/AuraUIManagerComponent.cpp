#include "UI/AuraUIManagerComponent.h"

#include "AuraGameplayTags.h"
#include "CommonActivatableWidget.h"
#include "CommonUIExtensions.h"
#include "GameFramework/PlayerController.h"
#include "UI/AuraUIWidgetConfig.h"

void UAuraUIManagerComponent::ToggleUI(FGameplayTag Tag) {
  if (ActiveWidgets.Contains(Tag)) {
    DisableUI(Tag);
  } else {
    EnableUI(Tag);
  }
}

void UAuraUIManagerComponent::EnableUI(FGameplayTag Tag) {
  if (ActiveWidgets.Contains(Tag)) {
    return;
  }

  UCommonActivatableWidget* Widget = CreateAndPush(Tag);
  if (Widget) {
    ActiveWidgets.Add(Tag, Widget);
  }
}

void UAuraUIManagerComponent::DisableUI(FGameplayTag Tag) {
  if (UCommonActivatableWidget* Widget = ActiveWidgets.FindRef(Tag)) {
    Widget->DeactivateWidget();
    ActiveWidgets.Remove(Tag);
  }
}

UCommonActivatableWidget* UAuraUIManagerComponent::CreateAndPush(
    FGameplayTag Tag) {
  if (!WidgetConfig) {
    return nullptr;
  }

  const FAuraUIWidgetEntry* Entry = WidgetConfig->FindEntry(Tag);
  if (!Entry || !Entry->WidgetClass) {
    return nullptr;
  }

  APlayerController* PC = nullptr;

  if (APawn* Pawn = Cast<APawn>(GetOwner())) {
    PC = Cast<APlayerController>(Pawn->GetController());
  } else {
    PC = GetOwner()->GetInstigatorController<APlayerController>();
  }

  if (!PC || !PC->GetLocalPlayer()) {
    UE_LOG(LogTemp, Warning, TEXT("UIManager: Invalid PC/LocalPlayer"));
    return nullptr;
  }

  UE_LOG(LogTemp, Warning, TEXT("Pushing widget for tag: %s"), *Tag.ToString());
  return UCommonUIExtensions::PushContentToLayer_ForPlayer(
      PC->GetLocalPlayer(), Entry->LayerTag, Entry->WidgetClass);
}