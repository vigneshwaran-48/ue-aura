#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AuraEquipmentManagerComponent.generated.h"

class UAuraEquipmentDefinition;
class UAuraEquipmentInstance;
class UAuraAbilitySystemComponent;

UENUM(BlueprintType)
enum class EAuraEquipmentType : uint8 { Primary, Secondary, Passive };

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AURA_API UAuraEquipmentManagerComponent : public UActorComponent {
  GENERATED_BODY()

 public:
  UAuraEquipmentManagerComponent();

 protected:
  virtual void BeginPlay() override;

 private:
  UPROPERTY()
  UAuraEquipmentInstance* CurrentPrimaryEquipment = nullptr;

  UPROPERTY()
  UAuraEquipmentInstance* CurrentSecondaryEquipment = nullptr;

  UPROPERTY()
  TMap<FGameplayTag, UAuraEquipmentInstance*> PassiveEquipments;

 public:
  UFUNCTION(BlueprintCallable, Category = "Equipment")
  UAuraEquipmentInstance* EquipItem(
      const UAuraEquipmentDefinition* EquipmentDefinition);

  UFUNCTION(BlueprintCallable, Category = "Equipment")
  void UnequipPrimary();

  UFUNCTION(BlueprintCallable, Category = "Equipment")
  void UnequipSecondary();

  UFUNCTION(BlueprintCallable, Category = "Equipment")
  void UnequipPassive(FGameplayTag PassiveTag);

 private:
  UAuraAbilitySystemComponent* GetASC() const;

  void EquipPrimary(UAuraEquipmentInstance* NewInstance,
                    UAuraAbilitySystemComponent* ASC);
  void EquipSecondary(UAuraEquipmentInstance* NewInstance,
                      UAuraAbilitySystemComponent* ASC);
  void EquipPassive(FGameplayTag PassiveTag,
                    UAuraEquipmentInstance* NewInstance,
                    UAuraAbilitySystemComponent* ASC);

  void CleanupInstance(UAuraEquipmentInstance* Instance,
                       UAuraAbilitySystemComponent* ASC);
};