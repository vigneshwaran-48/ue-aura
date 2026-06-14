#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Animation/Overlay/AuraOverlayComponent.h"
#include "AuraAbilitySystemComponent.h"
#include "Character/AuraPawnData.h"
#include "Equipment/AuraEquipmentManagerComponent.h"
#include "GameplayTagContainer.h"
#include "Input/AuraEnhancedInputComponent.h"
#include "Input/AuraInputConfig.h"
#include "Interaction/AuraInteractionComponent.h"
#include "Inventory/AuraInventoryComponent.h"
#include "UI/AuraUIManagerComponent.h"

AAuraCharacter::AAuraCharacter() {
  PrimaryActorTick.bCanEverTick = true;

  // Instantiate all framework components matching the Pawn setup
  AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(
      "AbilitySystemComponent");
  EquipmentManager = CreateDefaultSubobject<UAuraEquipmentManagerComponent>(
      "EquipmentManager");
  InteractionComponent =
      CreateDefaultSubobject<UAuraInteractionComponent>("InteractionComponent");
  InventoryComponent =
      CreateDefaultSubobject<UAuraInventoryComponent>("InventoryComponent");
  OverlayComponent =
      CreateDefaultSubobject<UAuraOverlayComponent>("OverlayComponent");
  UIManagerComponent =
      CreateDefaultSubobject<UAuraUIManagerComponent>("UIManagerComponent");

  // Initialize specific collision settings for tracking
  if (InteractionComponent) {
    InteractionComponent->SetInteractionTraceChannel(InteractionTraceChannel);
  }
}

void AAuraCharacter::BeginPlay() { Super::BeginPlay(); }

void AAuraCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (AbilitySystemComponent && IsLocallyControlled()) {
    AbilitySystemComponent->ProcessAbilityInput(DeltaTime, false);
  }
}

UAbilitySystemComponent *AAuraCharacter::GetAbilitySystemComponent() const {
  return AbilitySystemComponent;
}

void AAuraCharacter::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  UAuraEnhancedInputComponent *AuraInputComponent =
      CastChecked<UAuraEnhancedInputComponent>(PlayerInputComponent);

  if (!PawnData || !PawnData->InputConfig) {
    return;
  }

  // Bind Ability Actions directly using the InputConfig defined inside the
  // structured Pawn Data
  AuraInputComponent->BindAbilityActions(PawnData->InputConfig, this,
                                         &AAuraCharacter::InputAbilityPressed,
                                         &AAuraCharacter::InputAbilityReleased);
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

void AAuraCharacter::PossessedBy(AController *NewController) {
  Super::PossessedBy(NewController);

  // Server-side entry point for character capability initialization
  InitializeFromPawnData();
}

void AAuraCharacter::UnPossessed() { Super::UnPossessed(); }

void AAuraCharacter::InitializeFromPawnData() { InitializeAbilities(); }

void AAuraCharacter::InitializeAbilities() {
  if (!AbilitySystemComponent || !PawnData)
    return;

  for (const UAuraAbilitySet *Set : PawnData->AbilitySets) {
    if (Set) {
      // FIXED: Passing by reference directly
      Set->GiveToAbilitySystem(AbilitySystemComponent, AbilitySetHandles);
    }
  }
}

USceneComponent *AAuraCharacter::GetEquipmentAttachComponent_Implementation(
    FName SocketName) const {
  // ACharacter gives us a native SkeletalMeshComponent pointer via GetMesh()
  // without running FindComponent queries.
  if (USkeletalMeshComponent *SkelMesh = GetMesh()) {
    return SkelMesh;
  }

  if (USceneComponent *SceneComp = GetRootComponent()) {
    UE_LOG(LogTemp, Warning,
           TEXT("Using RootComponent as fallback attach for %s"),
           *GetNameSafe(this));
    return SceneComp;
  }

  UE_LOG(LogTemp, Error, TEXT("No valid attach component found on %s"),
         *GetNameSafe(this));
  return nullptr;
}

const UAuraPawnData *AAuraCharacter::GetPawnData_Implementation() const {
  return PawnData;
}
