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

  const FGameplayTagContainer& NewSlots = EquipmentDefinition->SlotTags;

  for (int32 i = 0; i < EquippedItems.Num(); i++) {
    UAuraEquipmentInstance* Existing = EquippedItems[i];

    if (Existing->GetEquipmentDefinition()->SlotTags.HasAny(NewSlots)) {
      Existing->OnUnequipped(ASC);
      EquippedItems.RemoveAt(i);
    }
  }

  UAuraEquipmentInstance* NewInstance = NewObject<UAuraEquipmentInstance>(
      GetOwner(), EquipmentDefinition->InstanceType);

  NewInstance->Initialize(
      const_cast<UAuraEquipmentDefinition*>(EquipmentDefinition), GetOwner());

  NewInstance->OnEquipped(ASC);

  EquippedItems.Add(NewInstance);

  return NewInstance;
}

void UAuraEquipmentManagerComponent::UnequipItemBySlot(FGameplayTag SlotTag) {
  UAuraAbilitySystemComponent* ASC = GetASC();

  for (int32 i = EquippedItems.Num() - 1; i >= 0; --i) {
    if (EquippedItems[i]->GetEquipmentDefinition()->SlotTags.HasTag(SlotTag)) {
      EquippedItems[i]->OnUnequipped(ASC);
      EquippedItems.RemoveAt(i);
    }
  }
}