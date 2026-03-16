#include "Character/AuraPlayerController.h"

#include "AbilitySystemInterface.h"
#include "AuraAbilitySystemComponent.h"
#include "CommonLocalPlayer.h"
#include "PrimaryGameLayout.h"
#include "UI/AuraUIExtensions.h"

AAuraPlayerController::AAuraPlayerController(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {}

void AAuraPlayerController::BeginPlay() {
  Super::BeginPlay();

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
  /*
   * The code inside this debug block isn't really required for this to work.
   * However, it can be easy to miss these setup steps, so here are some
   * extra checks to ensure the INI is correctly configured.
   */

  // CommonGame requires that the LocalPlayer is a UCommonLocalPlayer
  const UCommonLocalPlayer* LocalPlayer =
      Cast<UCommonLocalPlayer>(GetLocalPlayer());
  ensureAlwaysMsgf(IsValid(LocalPlayer),
                   TEXT("LocalPlayer must be derived from UCommonLocalPlayer; "
                        "fix INI and restart"));

  // Make sure we're configured with a valid RootUILayout
  const UPrimaryGameLayout* RootUILayout = LocalPlayer->GetRootUILayout();
  ensureAlwaysMsgf(IsValid(RootUILayout),
                   TEXT("RootUILayout must be configured in project settings"));
#endif
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
