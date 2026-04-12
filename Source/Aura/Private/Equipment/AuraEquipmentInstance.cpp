#include "Equipment/AuraEquipmentInstance.h"

#include "AuraAbilitySystemComponent.h"
#include "Equipment/AuraEquipmentDefinition.h"
#include "Equipment/AuraEquipmentInterface.h"
#include "GameFramework/Character.h"

void UAuraEquipmentInstance::Initialize(UAuraEquipmentDefinition* InDefinition,
                                        AActor* InOwner) {
  EquipmentDefinition = InDefinition;
  OwningActor = InOwner;
}

void UAuraEquipmentInstance::OnEquipped_Implementation(
    UAuraAbilitySystemComponent* ASC) {
  if (!EquipmentDefinition) return;

  if (EquipmentDefinition->AbilitySet) {
    EquipmentDefinition->AbilitySet->GiveToAbilitySystem(ASC, GrantedHandles);
  }

  if (EquipmentDefinition->EquipmentActorToSpawn) {
    if (!EquipmentDefinition || !EquipmentDefinition->EquipmentActorToSpawn) {
      UE_LOG(LogTemp, Error,
             TEXT("Invalid EquipmentDefinition or ActorToSpawn"));
      return;
    }

    if (!OwningActor) {
      UE_LOG(LogTemp, Error, TEXT("OwningActor is null"));
      return;
    }

    UWorld* World = OwningActor->GetWorld();
    if (!World) {
      UE_LOG(LogTemp, Error, TEXT("World is null for %s"),
             *GetNameSafe(OwningActor));
      return;
    }

    SpawnedActor =
        World->SpawnActor<AActor>(EquipmentDefinition->EquipmentActorToSpawn);
    if (!SpawnedActor) {
      UE_LOG(LogTemp, Error, TEXT("Failed to spawn equipment actor"));
      return;
    }

    USceneComponent* AttachComp = nullptr;

    if (OwningActor && OwningActor->Implements<UAuraEquipmentInterface>()) {
      AttachComp = IAuraEquipmentInterface::Execute_GetEquipmentAttachComponent(
          OwningActor, EquipmentDefinition->AttachSocket);
    }

    if (!AttachComp) {
      UE_LOG(LogTemp, Error, TEXT("Attach component not resolved for %s"),
             *GetNameSafe(OwningActor));
      return;
    }

    SpawnedActor->AttachToComponent(
        AttachComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        EquipmentDefinition->AttachSocket);
  }
}

void UAuraEquipmentInstance::OnUnequipped_Implementation(
    UAuraAbilitySystemComponent* ASC) {
  if (ASC) {
    GrantedHandles.TakeFromAbilitySystem(ASC);
  }

  if (SpawnedActor) {
    SpawnedActor->Destroy();
    SpawnedActor = nullptr;
  }
}