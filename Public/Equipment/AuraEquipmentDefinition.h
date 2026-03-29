#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AuraEquipmentDefinition.generated.h"

class UAuraAbilitySet;
class UAuraEquipmentInstance;

UCLASS(BlueprintType)
class AURA_API UAuraEquipmentDefinition : public UPrimaryDataAsset {
  GENERATED_BODY()

 public:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSubclassOf<UAuraEquipmentInstance> InstanceType;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TObjectPtr<const UAuraAbilitySet> AbilitySet;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSubclassOf<AActor> EquipmentActorToSpawn;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FName AttachSocket;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Aura.Slots"))
  FGameplayTagContainer SlotTags;
};