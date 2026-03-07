#include "Equipment/AuraEquipmentManagerComponent.h"

#include "AbilitySystemInterface.h"
#include "AuraAbilitySystemComponent.h"
#include "Equipment/AuraEquipmentDefinition.h"
#include "Equipment/AuraEquipmentInstance.h"

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

  const FGameplayTag SlotTag = EquipmentDefinition->SlotTag;

  UAuraEquipmentInstance* OldInstance = nullptr;

  if (EquippedItems.Contains(SlotTag)) {
    OldInstance = EquippedItems[SlotTag];

    OldInstance->OnUnequipped(ASC);

    EquippedItems.Remove(SlotTag);
  }

  UAuraEquipmentInstance* NewInstance = NewObject<UAuraEquipmentInstance>(
      GetOwner(), EquipmentDefinition->InstanceType);

  NewInstance->Initialize(
      const_cast<UAuraEquipmentDefinition*>(EquipmentDefinition), GetOwner());

  NewInstance->OnEquipped(ASC);

  EquippedItems.Add(SlotTag, NewInstance);

  return OldInstance;
}

void UAuraEquipmentManagerComponent::UnequipItem(FGameplayTag SlotTag) {
  if (!EquippedItems.Contains(SlotTag)) return;

  UAuraAbilitySystemComponent* ASC = GetASC();

  UAuraEquipmentInstance* Instance = EquippedItems[SlotTag];

  Instance->OnUnequipped(ASC);

  EquippedItems.Remove(SlotTag);
}