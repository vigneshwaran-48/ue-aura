#pragma once

#include "AbilitySystem/AuraAbilitySet.h"
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AuraEquipmentInstance.generated.h"

class UAuraEquipmentDefinition;
class UAuraAbilitySystemComponent;

UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraEquipmentInstance : public UObject {
  GENERATED_BODY()

 public:
  void Initialize(UAuraEquipmentDefinition* InDefinition, AActor* InOwner);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment")
  void OnEquipped(UAuraAbilitySystemComponent* ASC);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment")
  void OnUnequipped(UAuraAbilitySystemComponent* ASC);

  virtual void OnEquipped_Implementation(UAuraAbilitySystemComponent* ASC);

  virtual void OnUnequipped_Implementation(UAuraAbilitySystemComponent* ASC);

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

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
            Category = "Equipment|Attachment")
  FName AttachComponentTag = TEXT("EquipmentAttachableMesh");
};