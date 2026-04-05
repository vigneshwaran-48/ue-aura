#pragma once

#include "AbilitySystem/AuraAbilitySet.h"
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AuraEquipmentInstance.generated.h"

class UAuraEquipmentDefinition;
class UAuraAbilitySystemComponent;

UCLASS(BlueprintType)
class AURA_API UAuraEquipmentInstance : public UObject {
  GENERATED_BODY()

 public:
  void Initialize(UAuraEquipmentDefinition* InDefinition, AActor* InOwner);

  void OnEquipped(UAuraAbilitySystemComponent* ASC);
  void OnUnequipped(UAuraAbilitySystemComponent* ASC);

  TObjectPtr<UAuraEquipmentDefinition> GetEquipmentDefinition() {
    return EquipmentDefinition;
  }

 protected:
  UPROPERTY()
  TObjectPtr<UAuraEquipmentDefinition> EquipmentDefinition;

  UPROPERTY()
  TObjectPtr<AActor> SpawnedActor;

  UPROPERTY()
  FAuraAbilitySet_GrantedHandles GrantedHandles;

  UPROPERTY()
  TObjectPtr<AActor> OwningActor;
};