#include "Equipment/AuraEquipmentManagerComponent.h"

#include "AbilitySystemInterface.h"
#include "AuraAbilitySystemComponent.h"
#include "Equipment/AuraEquipmentDefinition.h"
#include "Equipment/AuraEquipmentInstance.h"

UAuraEquipmentManagerComponent::UAuraEquipmentManagerComponent() {
  PrimaryComponentTick.bCanEverTick = false;
}

void UAuraEquipmentManagerComponent::BeginPlay() { Super::BeginPlay(); }

UAuraAbilitySystemComponent* UAuraEquipmentManagerComponent::GetASC() const {
  IAbilitySystemInterface* ASCInterface =
      Cast<IAbilitySystemInterface>(GetOwner());
  if (!ASCInterface) return nullptr;

  return Cast<UAuraAbilitySystemComponent>(
      ASCInterface->GetAbilitySystemComponent());
}

UAuraEquipmentInstance* UAuraEquipmentManagerComponent::EquipItem(
    const UAuraEquipmentDefinition* EquipmentDefinition) {
  if (!EquipmentDefinition) return nullptr;

  UAuraAbilitySystemComponent* ASC = GetASC();
  if (!ASC) return nullptr;

  UAuraEquipmentInstance* NewInstance = NewObject<UAuraEquipmentInstance>(
      GetOwner(), EquipmentDefinition->InstanceType);

  NewInstance->Initialize(
      const_cast<UAuraEquipmentDefinition*>(EquipmentDefinition), GetOwner());

  switch (EquipmentDefinition->EquipmentType) {
    case EAuraEquipmentType::Primary:
      EquipPrimary(NewInstance, ASC);
      break;

    case EAuraEquipmentType::Secondary:
      EquipSecondary(NewInstance, ASC);
      break;

    case EAuraEquipmentType::Passive:
      EquipPassive(EquipmentDefinition->PassiveSlotTag, NewInstance, ASC);
      break;
  }

  return NewInstance;
}

void UAuraEquipmentManagerComponent::EquipPrimary(
    UAuraEquipmentInstance* NewInstance, UAuraAbilitySystemComponent* ASC) {
  if (CurrentPrimaryEquipment) {
    CleanupInstance(CurrentPrimaryEquipment, ASC);
    CurrentPrimaryEquipment = nullptr;
  }

  CurrentPrimaryEquipment = NewInstance;
  NewInstance->OnEquipped(ASC);
}

void UAuraEquipmentManagerComponent::EquipSecondary(
    UAuraEquipmentInstance* NewInstance, UAuraAbilitySystemComponent* ASC) {
  if (CurrentSecondaryEquipment) {
    CleanupInstance(CurrentSecondaryEquipment, ASC);
    CurrentSecondaryEquipment = nullptr;
  }

  CurrentSecondaryEquipment = NewInstance;
  NewInstance->OnEquipped(ASC);
}

void UAuraEquipmentManagerComponent::EquipPassive(
    FGameplayTag PassiveTag, UAuraEquipmentInstance* NewInstance,
    UAuraAbilitySystemComponent* ASC) {
  if (PassiveEquipments.Contains(PassiveTag)) {
    CleanupInstance(PassiveEquipments[PassiveTag], ASC);
    PassiveEquipments.Remove(PassiveTag);
  }

  PassiveEquipments.Add(PassiveTag, NewInstance);
  NewInstance->OnEquipped(ASC);
}

void UAuraEquipmentManagerComponent::UnequipPrimary() {
  UAuraAbilitySystemComponent* ASC = GetASC();
  if (!ASC || !CurrentPrimaryEquipment) return;

  CleanupInstance(CurrentPrimaryEquipment, ASC);
  CurrentPrimaryEquipment = nullptr;
}

void UAuraEquipmentManagerComponent::UnequipSecondary() {
  UAuraAbilitySystemComponent* ASC = GetASC();
  if (!ASC || !CurrentSecondaryEquipment) return;

  CleanupInstance(CurrentSecondaryEquipment, ASC);
  CurrentSecondaryEquipment = nullptr;
}

void UAuraEquipmentManagerComponent::UnequipPassive(FGameplayTag PassiveTag) {
  UAuraAbilitySystemComponent* ASC = GetASC();
  if (!ASC) return;

  if (UAuraEquipmentInstance** Found = PassiveEquipments.Find(PassiveTag)) {
    CleanupInstance(*Found, ASC);
    PassiveEquipments.Remove(PassiveTag);
  }
}

void UAuraEquipmentManagerComponent::CleanupInstance(
    UAuraEquipmentInstance* Instance, UAuraAbilitySystemComponent* ASC) {
  if (!Instance) return;

  Instance->OnUnequipped(ASC);
}