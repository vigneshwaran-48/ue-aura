#include "Character/AuraPawn.h"

#include "AbilitySystem/AuraAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "Animation/Overlay/AuraOverlayComponent.h"
#include "AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Character/AuraPawnData.h"
#include "CommonUIExtensions.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Equipment/AuraEquipmentManagerComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Input/AuraEnhancedInputComponent.h"
#include "Input/AuraInputConfig.h"
#include "Interaction/AuraInteractionComponent.h"
#include "UI/AuraHUDLayout.h"

AAuraPawn::AAuraPawn() {
  PrimaryActorTick.bCanEverTick = true;

  AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(
      "AbilitySystemComponent");
  EquipmentManager = CreateDefaultSubobject<UAuraEquipmentManagerComponent>(
      "EquipmentManager");
  InteractionComponent =
      CreateDefaultSubobject<UAuraInteractionComponent>("InteractionComponent");
  OverlayComponent =
      CreateDefaultSubobject<UAuraOverlayComponent>("OverlayComponent");

  InteractionComponent->SetInteractionTraceChannel(InteractionTraceChannel);
}

void AAuraPawn::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (AbilitySystemComponent && IsLocallyControlled()) {
    AbilitySystemComponent->ProcessAbilityInput(DeltaTime, false);
  }
}

UAbilitySystemComponent* AAuraPawn::GetAbilitySystemComponent() const {
  return AbilitySystemComponent;
}

void AAuraPawn::BeginPlay() { Super::BeginPlay(); }

void AAuraPawn::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  UAuraEnhancedInputComponent* AuraInputComponent =
      CastChecked<UAuraEnhancedInputComponent>(PlayerInputComponent);

  if (PawnData && PawnData->InputConfig) {
    AuraInputComponent->BindAbilityActions(PawnData->InputConfig, this,
                                           &AAuraPawn::InputAbilityPressed,
                                           &AAuraPawn::InputAbilityReleased);
  }
}

void AAuraPawn::InputAbilityPressed(FGameplayTag InputTag) {
  if (AbilitySystemComponent) {
    AbilitySystemComponent->AbilityInputTagPressed(InputTag);
  }
}

void AAuraPawn::InputAbilityReleased(FGameplayTag InputTag) {
  if (AbilitySystemComponent) {
    AbilitySystemComponent->AbilityInputTagReleased(InputTag);
  }
}

void AAuraPawn::PossessedBy(AController* NewController) {
  Super::PossessedBy(NewController);
  InitializeFromPawnData();
}

void AAuraPawn::UnPossessed() {
  // Remove any HUD we added to the player's UI
  if (HUDLayoutWidget.IsValid()) {
    UE_LOG(LogTemp, Log, TEXT("Cleaning up HUD Layout Widget"));
    UCommonUIExtensions::PopContentFromLayer(HUDLayoutWidget.Get());
    HUDLayoutWidget.Reset();
  }

  Super::UnPossessed();
}

void AAuraPawn::InitializeFromPawnData() {
  InitializeAbilities();
  InitializeInput();
  InitializeUI();
}

void AAuraPawn::InitializeAbilities() {
  if (!AbilitySystemComponent || !PawnData) return;

  for (const UAuraAbilitySet* Set : PawnData->AbilitySets) {
    if (Set) {
      Set->GiveToAbilitySystem(AbilitySystemComponent, AbilitySetHandles);
    }
  }
}

void AAuraPawn::InitializeInput() {
  if (!PawnData) return;

  if (APlayerController* PC = Cast<APlayerController>(GetController())) {
    if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer()) {
      if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
              LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
        if (PawnData->InputMappingContext) {
          Subsystem->AddMappingContext(PawnData->InputMappingContext, 0);
        }
      }
    }
  }
}

void AAuraPawn::InitializeUI() {
  if (!PawnData) return;

  const APlayerController* PC = Cast<APlayerController>(GetController());
  if (!PC) return;

  if (PawnData->HUDLayoutClass) {
    HUDLayoutWidget = UCommonUIExtensions::PushContentToLayer_ForPlayer(
        PC->GetLocalPlayer(), TAG_UI_Layer_Game, PawnData->HUDLayoutClass);
  }
}

USceneComponent* AAuraPawn::GetEquipmentAttachComponent_Implementation(
    FName SocketName) const {
  if (USkeletalMeshComponent* SkelMesh =
          FindComponentByClass<USkeletalMeshComponent>()) {
    return SkelMesh;
  }

  if (USceneComponent* SceneComp = GetRootComponent()) {
    UE_LOG(LogTemp, Warning,
           TEXT("Using RootComponent as fallback attach for %s"),
           *GetNameSafe(this));

    return SceneComp;
  }

  UE_LOG(LogTemp, Error, TEXT("No valid attach component found on %s"),
         *GetNameSafe(this));

  return nullptr;
}
