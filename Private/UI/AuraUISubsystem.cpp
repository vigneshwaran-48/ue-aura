#include "UI/AuraUISubsystem.h"

#include "GameFramework/PlayerController.h"
#include "UI/AuraPrimaryGameLayout.h"
#include "UI/AuraUIPolicy.h"

void UAuraUISubsystem::InitializeUI(APlayerController* PlayerController) {
  if (!PlayerController) {
    return;
  }

  if (!UIPolicy) {
    UIPolicy = NewObject<UAuraUIPolicy>(this);
  }

  UIPolicy->Initialize(PlayerController);
}

UAuraPrimaryGameLayout* UAuraUISubsystem::GetRootLayout() const {
  if (!UIPolicy) {
    return nullptr;
  }

  return UIPolicy->GetRootLayout();
}