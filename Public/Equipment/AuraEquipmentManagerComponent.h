#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AuraEquipmentManagerComponent.generated.h"

class UAuraEquipmentDefinition;
class UAuraEquipmentInstance;
class UAuraAbilitySystemComponent;

UCLASS(ClassGroup = (Aura), meta = (BlueprintSpawnableComponent))
class AURA_API UAuraEquipmentManagerComponent : public UActorComponent {
  GENERATED_BODY()

 public:
  UAuraEquipmentInstance* EquipItem(
      const UAuraEquipmentDefinition* EquipmentDefinition);

  void UnequipItem(FGameplayTag Slot);

 protected:
  UPROPERTY()
  TMap<FGameplayTag, TObjectPtr<UAuraEquipmentInstance>> EquippedItems;

  UAuraAbilitySystemComponent* GetASC() const;
};