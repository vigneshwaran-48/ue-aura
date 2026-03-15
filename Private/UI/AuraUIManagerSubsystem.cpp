#include "UI/AuraUIManagerSubsystem.h"

#include "CommonLocalPlayer.h"
#include "Engine/GameInstance.h"
#include "GameFramework/HUD.h"
#include "GameUIPolicy.h"
#include "PrimaryGameLayout.h"

UAuraUIManagerSubsystem::UAuraUIManagerSubsystem() {}

void UAuraUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
  Super::Initialize(Collection);

  TickHandle = FTSTicker::GetCoreTicker().AddTicker(
      FTickerDelegate::CreateUObject(this, &UAuraUIManagerSubsystem::Tick),
      0.0f);
}

void UAuraUIManagerSubsystem::Deinitialize() {
  FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);

  Super::Deinitialize();
}

bool UAuraUIManagerSubsystem::Tick(float DeltaTime) {
  SyncRootLayoutVisibilityToShowHUD();

  return true;
}

void UAuraUIManagerSubsystem::SyncRootLayoutVisibilityToShowHUD() {
  if (const UGameUIPolicy* Policy = GetCurrentUIPolicy()) {
    for (const ULocalPlayer* LocalPlayer :
         GetGameInstance()->GetLocalPlayers()) {
      bool bShouldShowUI = true;

      if (const APlayerController* PC =
              LocalPlayer->GetPlayerController(GetWorld())) {
        const AHUD* HUD = PC->GetHUD();

        if (HUD && !HUD->bShowHUD) {
          bShouldShowUI = false;
        }
      }

      if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(
              CastChecked<UCommonLocalPlayer>(LocalPlayer))) {
        const ESlateVisibility DesiredVisibility =
            bShouldShowUI ? ESlateVisibility::SelfHitTestInvisible
                          : ESlateVisibility::Collapsed;
        if (DesiredVisibility != RootLayout->GetVisibility()) {
          RootLayout->SetVisibility(DesiredVisibility);
        }
      }
    }
  }
}