#include "Character/AuraCharacter.h"

#include "AbilitySystem/AuraAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Input/AuraEnhancedInputComponent.h"
#include "Input/AuraInputConfig.h"
#include "Equipment/AuraEquipmentManagerComponent.h"

AAuraCharacter::AAuraCharacter() {
  PrimaryActorTick.bCanEverTick = false;

  AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(
      "AbilitySystemComponent");
  EquipmentManager = CreateDefaultSubobject<UAuraEquipmentManagerComponent>(
      "EquipmentManager");
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