#include "Character/AuraPawn.h"

#include "AbilitySystem/AuraAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "Animation/Overlay/AuraOverlayComponent.h"
#include "AuraAbilitySystemComponent.h"
#include "Character/AuraPawnData.h"
#include "CommonUIExtensions.h"
#include "Equipment/AuraEquipmentManagerComponent.h"
#include "GameFramework/PlayerController.h"
#include "Input/AuraEnhancedInputComponent.h"
#include "Interaction/AuraInteractionComponent.h"
#include "Inventory/AuraInventoryComponent.h"
#include "UI/AuraUIManagerComponent.h"

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
  InventoryComponent =
      CreateDefaultSubobject<UAuraInventoryComponent>("InventoryComponent");
  UIManagerComponent =
      CreateDefaultSubobject<UAuraUIManagerComponent>("UIManagerComponent");

  InteractionComponent->SetInteractionTraceChannel(InteractionTraceChannel);
}

void AAuraPawn::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (AbilitySystemComponent && IsLocallyControlled()) {
    AbilitySystemComponent->ProcessAbilityInput(DeltaTime, false);
  }
}

UAbilitySystemComponent *AAuraPawn::GetAbilitySystemComponent() const {
  return AbilitySystemComponent;
}

void AAuraPawn::BeginPlay() { Super::BeginPlay(); }

void AAuraPawn::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  UAuraEnhancedInputComponent *AuraInputComponent =
      CastChecked<UAuraEnhancedInputComponent>(PlayerInputComponent);

  if (!PawnData || !PawnData->InputConfig) {
    return;
  }

  AuraInputComponent->BindAbilityActions(PawnData->InputConfig, this,
                                         &AAuraPawn::InputAbilityPressed,
                                         &AAuraPawn::InputAbilityReleased);
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

void AAuraPawn::PossessedBy(AController *NewController) {
  Super::PossessedBy(NewController);
  InitializeFromPawnData();
}

void AAuraPawn::UnPossessed() {
  // Remove any HUD we added to the player's UI
  Super::UnPossessed();
}

void AAuraPawn::InitializeFromPawnData() { InitializeAbilities(); }

void AAuraPawn::InitializeAbilities() {
  if (!AbilitySystemComponent || !PawnData)
    return;

  for (const UAuraAbilitySet *Set : PawnData->AbilitySets) {
    if (Set) {
      Set->GiveToAbilitySystem(AbilitySystemComponent, AbilitySetHandles);
    }
  }
}

USceneComponent *
AAuraPawn::GetEquipmentAttachComponent_Implementation(FName SocketName) const {
  if (USkeletalMeshComponent *SkelMesh =
          FindComponentByClass<USkeletalMeshComponent>()) {
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

const UAuraPawnData *AAuraPawn::GetPawnData_Implementation() const {
  return PawnData;
}
