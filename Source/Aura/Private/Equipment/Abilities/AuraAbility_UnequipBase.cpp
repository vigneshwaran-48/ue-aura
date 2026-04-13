#include "Equipment/Abilities/AuraAbility_UnequipBase.h"

#include "AbilitySystemComponent.h"
#include "Equipment/AuraEquipmentManagerComponent.h"
#include "GameFramework/Actor.h"

UAuraAbility_UnequipBase::UAuraAbility_UnequipBase() {
  InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

UAuraEquipmentManagerComponent* UAuraAbility_UnequipBase::GetEquipmentComponent(
    const FGameplayAbilityActorInfo* ActorInfo) const {
  if (!ActorInfo || !ActorInfo->AvatarActor.IsValid()) {
    return nullptr;
  }

  return ActorInfo->AvatarActor
      ->FindComponentByClass<UAuraEquipmentManagerComponent>();
}

void UAuraAbility_UnequipBase::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData) {
  UAuraEquipmentManagerComponent* EquipComp = GetEquipmentComponent(ActorInfo);

  if (!EquipComp) {
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    return;
  }

  switch (EquipmentType) {
    case EAuraEquipmentType::Primary: {
      EquipComp->UnequipPrimary();
      break;
    }

    case EAuraEquipmentType::Secondary: {
      EquipComp->UnequipSecondary();
      break;
    }

    case EAuraEquipmentType::Passive: {
      if (TriggerEventData) {
        const FGameplayTag PassiveTag = TriggerEventData->EventTag;

        if (PassiveTag.IsValid()) {
          EquipComp->UnequipPassive(PassiveTag);
        }
      }
      break;
    }
  }

  EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}