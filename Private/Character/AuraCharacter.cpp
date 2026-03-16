#include "Character/AuraCharacter.h"

#include "AbilitySystem/AuraAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
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

AAuraCharacter::AAuraCharacter() {
  PrimaryActorTick.bCanEverTick = true;

  AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(
      "AbilitySystemComponent");
  EquipmentManager = CreateDefaultSubobject<UAuraEquipmentManagerComponent>(
      "EquipmentManager");
  InteractionComponent =
      CreateDefaultSubobject<UAuraInteractionComponent>("InteractionComponent");
}

UAbilitySystemComponent* AAuraCharacter::GetAbilitySystemComponent() const {
  return AbilitySystemComponent;
}

void AAuraCharacter::BeginPlay() {
  Super::BeginPlay();

  if (DefaultAbilitySet) {
    DefaultAbilitySet->GiveToAbilitySystem(AbilitySystemComponent,
                                           AbilitySetHandles);
  }

  if (APlayerController* PC = Cast<APlayerController>(GetController())) {
    if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer()) {
      if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
              LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
        if (DefaultMappingContext) {
          Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
      }
    }
  }
}

void AAuraCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  UAuraEnhancedInputComponent* AuraInputComponent =
      CastChecked<UAuraEnhancedInputComponent>(PlayerInputComponent);

  if (InputConfig) {
    AuraInputComponent->BindAbilityActions(
        InputConfig, this, &AAuraCharacter::InputAbilityPressed,
        &AAuraCharacter::InputAbilityReleased);
  }
}

void AAuraCharacter::InputAbilityPressed(FGameplayTag InputTag) {
  if (AbilitySystemComponent) {
    AbilitySystemComponent->AbilityInputTagPressed(InputTag);
  }
}

void AAuraCharacter::InputAbilityReleased(FGameplayTag InputTag) {
  if (AbilitySystemComponent) {
    AbilitySystemComponent->AbilityInputTagReleased(InputTag);
  }
}

void AAuraCharacter::PossessedBy(AController* NewController) {
  Super::PossessedBy(NewController);

  UE_LOG(LogTemp, Log, TEXT("Inside character possessedby"));

  const APlayerController* PC = Cast<APlayerController>(NewController);
  if (ensure(PC)) {
    // Add HUD Layout widget to the player's Game UI Layer
    UE_LOG(LogTemp, Log, TEXT("Pushing Game HUD [%s] to UI"),
           *GetNameSafe(HUDLayoutClass));
    HUDLayoutWidget = UCommonUIExtensions::PushContentToLayer_ForPlayer(
        PC->GetLocalPlayer(), TAG_UI_Layer_Game, HUDLayoutClass);
  }
}

void AAuraCharacter::UnPossessed() {
  // Remove any HUD we added to the player's UI
  if (HUDLayoutWidget.IsValid()) {
    UE_LOG(LogTemp, Log, TEXT("Cleaning up HUD Layout Widget"));
    UCommonUIExtensions::PopContentFromLayer(HUDLayoutWidget.Get());
    HUDLayoutWidget.Reset();
  }

  Super::UnPossessed();
}

void AAuraCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (AbilitySystemComponent && IsLocallyControlled()) {
    AbilitySystemComponent->ProcessAbilityInput(DeltaTime, false);
  }
}
