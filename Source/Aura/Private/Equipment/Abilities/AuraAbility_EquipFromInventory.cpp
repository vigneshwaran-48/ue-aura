#include "Equipment/Abilities/AuraAbility_EquipFromInventory.h"
#include "Inventory/Actions/AuraItemActionContextObject.h"
#include "Equipment/AuraEquipmentManagerComponent.h"
#include "Inventory/AuraInventoryComponent.h"
#include "Inventory/AuraItemInstance.h"
#include "Equipment/Fragments/AuraItemFragment_EquippableItem.h"

void UAuraAbility_EquipFromInventory::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData) {

    if (!ActorInfo || !ActorInfo->AvatarActor.IsValid()) {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    UAuraEquipmentManagerComponent* EquipmentManager = ActorInfo->AvatarActor
        ->FindComponentByClass<UAuraEquipmentManagerComponent>();

    UAuraItemActionContextObject* ActionContext = GetItemActionContext();

	if (!EquipmentManager || !ActionContext) {
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

    FAuraItemInstance* ItemInstance = ActionContext->Inventory->FindItem(ActionContext->ItemHandle);
	if (!ItemInstance) {
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

    const UAuraItemFragment_EquippableItem* EquipmentDefinitionFrag  = ItemInstance->Definition->FindFragment<UAuraItemFragment_EquippableItem>();
	if (!EquipmentDefinitionFrag) {
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	EquipmentManager->EquipItem(EquipmentDefinitionFrag->EquipmentDefinition);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}