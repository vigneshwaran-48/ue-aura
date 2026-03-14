#include "UI/AuraUIPolicy.h"

#include "UI/AuraPrimaryGameLayout.h"

void UAuraUIPolicy::Initialize(
    APlayerController* PlayerController,
    TSubclassOf<UAuraPrimaryGameLayout> LayoutClass) {
  if (!PlayerController || !LayoutClass) {
    return;
  }

  if (RootLayout) {
    return;
  }

  RootLayout =
      CreateWidget<UAuraPrimaryGameLayout>(PlayerController, LayoutClass);

  if (RootLayout) {
    RootLayout->AddToViewport();
    UE_LOG(LogTemp, Warning, TEXT("Aura UI Root Layout Created"));
  }
}

UAuraPrimaryGameLayout* UAuraUIPolicy::GetRootLayout() const {
  return RootLayout;
}