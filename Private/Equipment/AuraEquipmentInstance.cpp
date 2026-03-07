#include "Equipment/AuraEquipmentInstance.h"

#include "AuraAbilitySystemComponent.h"
#include "Equipment/AuraEquipmentDefinition.h"
#include "GameFramework/Character.h"

void UAuraEquipmentInstance::Initialize(UAuraEquipmentDefinition* InDefinition,
                                        AActor* InOwner) {
  EquipmentDefinition = InDefinition;
  OwningActor = InOwner;
}

void UAuraEquipmentInstance::OnEquipped(UAuraAbilitySystemComponent* ASC) {
  if (!EquipmentDefinition) return;

  if (EquipmentDefinition->AbilitySet) {
    EquipmentDefinition->AbilitySet->GiveToAbilitySystem(ASC, GrantedHandles);
  }

  // NOTE: For now disabling spawning feature since I am going to use GASPALS.

  if (EquipmentDefinition->EquipmentActorToSpawn) {
    UWorld* World = OwningActor->GetWorld();

    SpawnedActor =
        World->SpawnActor<AActor>(EquipmentDefinition->EquipmentActorToSpawn);

    SpawnedActor->AttachToComponent(
        Cast<ACharacter>(OwningActor)->GetMesh(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        EquipmentDefinition->AttachSocket);
  }
}

void UAuraEquipmentInstance::OnUnequipped(UAuraAbilitySystemComponent* ASC) {
  GrantedHandles.TakeFromAbilitySystem(ASC);

  // NOTE: For now disabling spawning feature since I am going to use GASPALS.

  if (SpawnedActor) {
    SpawnedActor->Destroy();
    SpawnedActor = nullptr;
  }
}