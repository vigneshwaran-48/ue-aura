#include "Character/AuraPlayerController.h"

#include "AbilitySystemInterface.h"
#include "AuraAbilitySystemComponent.h"
#include "UI/AuraUIExtensions.h"

AAuraPlayerController::AAuraPlayerController(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {}

void AAuraPlayerController::BeginPlay() {
  Super::BeginPlay();

  for (const FAuraUILayoutEntry& Entry : InitialUILayouts) {
    if (!Entry.WidgetClass) continue;

    UAuraUIExtensions::PushWidgetToLayer(this, Entry.LayerTag,
                                         Entry.WidgetClass);
  }
}

void AAuraPlayerController::PostProcessInput(float DeltaTime,
                                             bool bGamePaused) {
  Super::PostProcessInput(DeltaTime, bGamePaused);

  if (UAuraAbilitySystemComponent* ASC = GetAbilitySystemComponent()) {
    ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
  }
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAbilitySystemComponent()
    const {
  APawn* MyPawn = GetPawn();
  if (!MyPawn) {
    return nullptr;
  }

  if (const IAbilitySystemInterface* ASI =
          Cast<IAbilitySystemInterface>(MyPawn)) {
    return Cast<UAuraAbilitySystemComponent>(ASI->GetAbilitySystemComponent());
  }

  return nullptr;
}
