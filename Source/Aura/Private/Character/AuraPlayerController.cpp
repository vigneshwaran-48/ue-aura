#include "Character/AuraPlayerController.h"

#include "AbilitySystemInterface.h"
#include "AuraAbilitySystemComponent.h"
#include "CommonLocalPlayer.h"
#include "PrimaryGameLayout.h"
#include "UI/AuraUIExtensions.h"

#include "AuraGameplayTags.h"
#include "Character/AuraPawnData.h"
#include "Character/AuraPawnDataProvider.h"
#include "CommonUIExtensions.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AAuraPlayerController::AAuraPlayerController(
    const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer) {}

void AAuraPlayerController::BeginPlay() {
  Super::BeginPlay();
  UE_LOG(LogTemp, Warning, TEXT("On BeginPlay controller"));
  UE_LOG(LogTemp, Warning,
       TEXT("Pawn At BeginPlay: %s"),
       *GetNameSafe(GetPawn()));

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
  /*
   * The code inside this debug block isn't really required for this to work.
   * However, it can be easy to miss these setup steps, so here are some
   * extra checks to ensure the INI is correctly configured.
   */

  // CommonGame requires that the LocalPlayer is a UCommonLocalPlayer
  const UCommonLocalPlayer *LocalPlayer =
      Cast<UCommonLocalPlayer>(GetLocalPlayer());
  ensureAlwaysMsgf(IsValid(LocalPlayer),
                   TEXT("LocalPlayer must be derived from UCommonLocalPlayer; "
                        "fix INI and restart"));

  // Make sure we're configured with a valid RootUILayout
  const UPrimaryGameLayout *RootUILayout = LocalPlayer->GetRootUILayout();
  ensureAlwaysMsgf(IsValid(RootUILayout),
                   TEXT("RootUILayout must be configured in project settings"));
#endif
}

void AAuraPlayerController::OnPossess(APawn *InPawn) {
  Super::OnPossess(InPawn);

  RemoveCurrentPawnPresentation();

  UE_LOG(LogTemp, Warning, TEXT("On posses controller"));

  if (InPawn && InPawn->Implements<UAuraPawnDataProvider>()) {

    const UAuraPawnData *PawnData =
        IAuraPawnDataProvider::Execute_GetPawnData(InPawn);

    ApplyPawnData(PawnData);
  }
}

void AAuraPlayerController::OnUnPossess() {
  RemoveCurrentPawnPresentation();

  Super::OnUnPossess();
}

UAuraAbilitySystemComponent *
AAuraPlayerController::GetAbilitySystemComponent() const {
  APawn *MyPawn = GetPawn();
  if (!MyPawn) {
    return nullptr;
  }

  if (const IAbilitySystemInterface *ASI =
          Cast<IAbilitySystemInterface>(MyPawn)) {
    return Cast<UAuraAbilitySystemComponent>(ASI->GetAbilitySystemComponent());
  }

  return nullptr;
}

void AAuraPlayerController::ApplyPawnData(const UAuraPawnData *PawnData) {
  if (!PawnData) {
    UE_LOG(LogTemp, Warning, TEXT("ApplyPawnData: PawnData is invalid!"));
    return;
  }

  ULocalPlayer *LocalPlayer = GetLocalPlayer();
  if (!LocalPlayer) {
    UE_LOG(LogTemp, Warning, TEXT("ApplyPawnData: LocalPlayer is invalid!"));
    return;
  }

  if (UEnhancedInputLocalPlayerSubsystem *InputSubsystem =
          LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {

    if (PawnData->InputMappingContext) {
      InputSubsystem->AddMappingContext(PawnData->InputMappingContext, 0);

      ActiveInputMappingContext = PawnData->InputMappingContext;
    }
  }

  if (PawnData->HUDLayoutClass) {
    ActiveHUDLayoutWidget = UCommonUIExtensions::PushContentToLayer_ForPlayer(
        LocalPlayer, TAG_UI_Layer_Game, PawnData->HUDLayoutClass);
  }
}

void AAuraPlayerController::RemoveCurrentPawnPresentation() {
  ULocalPlayer *LocalPlayer = GetLocalPlayer();

  if (LocalPlayer) {
    if (UEnhancedInputLocalPlayerSubsystem *InputSubsystem =
            LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {

      if (ActiveInputMappingContext) {
        InputSubsystem->RemoveMappingContext(ActiveInputMappingContext);

        ActiveInputMappingContext = nullptr;
      }
    }
  }

  if (ActiveHUDLayoutWidget.IsValid()) {
    UCommonUIExtensions::PopContentFromLayer(ActiveHUDLayoutWidget.Get());

    ActiveHUDLayoutWidget.Reset();
  }
}

void AAuraPlayerController::SetPawn(APawn* InPawn) {
  Super::SetPawn(InPawn);

  UE_LOG(LogTemp, Warning,
         TEXT("SetPawn Called: %s"),
         *GetNameSafe(InPawn));

  RemoveCurrentPawnPresentation();

  if (InPawn &&
      InPawn->Implements<UAuraPawnDataProvider>()) {

    const UAuraPawnData* PawnData =
        IAuraPawnDataProvider::Execute_GetPawnData(InPawn);

    ApplyPawnData(PawnData);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("Pawn doesn't implement AuraPawnDataProvider"));
  }
}
