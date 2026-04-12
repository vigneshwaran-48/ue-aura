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
  UFUNCTION(BlueprintCallable, Category = "Equipment")
  UAuraEquipmentInstance* EquipItem(
      const UAuraEquipmentDefinition* EquipmentDefinition);

  UFUNCTION(BlueprintCallable, Category = "Equipment")
  void UnequipItemBySlot(FGameplayTag Slot);

 protected:
  UPROPERTY()
  TArray<TObjectPtr<UAuraEquipmentInstance>> EquippedItems;

  UAuraAbilitySystemComponent* GetASC() const;
};