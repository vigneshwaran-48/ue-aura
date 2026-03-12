#include "UI/AuraUIPolicy.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "UI/AuraPrimaryGameLayout.h"
#include "UI/AuraUISettings.h"

void UAuraUIPolicy::Initialize(APlayerController* PlayerController) {
  if (!PlayerController) {
    return;
  }

  const UAuraUISettings* Settings = GetDefault<UAuraUISettings>();

  if (!Settings || !Settings->PrimaryGameLayoutClass) {
    UE_LOG(LogTemp, Error,
           TEXT("AuraUIPolicy: PrimaryGameLayoutClass not set in settings"));
    return;
  }

  if (RootLayout) {
    return;
  }

  RootLayout = CreateWidget<UAuraPrimaryGameLayout>(
      PlayerController, Settings->PrimaryGameLayoutClass);

  if (!RootLayout) {
    UE_LOG(LogTemp, Error, TEXT("AuraUIPolicy: Failed to create RootLayout"));
    return;
  }

  RootLayout->AddToViewport();
}

UAuraPrimaryGameLayout* UAuraUIPolicy::GetRootLayout() const {
  return RootLayout;
}