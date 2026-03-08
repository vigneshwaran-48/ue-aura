#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AuraAbilitySet.h"
#include "AuraCharacter.generated.h"

class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class UAuraInputComponent;
class UAbilitySystemComponent;
class UInputMappingContext;
class UAuraEquipmentManagerComponent;
class UAuraInteractionComponent;

UCLASS()
class AURA_API AAuraCharacter : public ACharacter,
                                public IAbilitySystemInterface {
  GENERATED_BODY()

 public:
  AAuraCharacter();

  virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

 protected:
  virtual void BeginPlay() override;

  virtual void SetupPlayerInputComponent(
      UInputComponent* PlayerInputComponent) override;

 protected:

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aura|GAS")
  TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;

  UPROPERTY(EditDefaultsOnly, Category = "Aura|Input")
  TObjectPtr<UInputMappingContext> DefaultMappingContext;

  UPROPERTY(EditDefaultsOnly, Category = "Aura|Input")
  TObjectPtr<UAuraInputConfig> InputConfig;

  UPROPERTY(EditDefaultsOnly, Category = "Aura|Ability System")
  UAuraAbilitySet* DefaultAbilitySet;

  UPROPERTY(VisibleAnywhere)
  TObjectPtr<UAuraInteractionComponent> InteractionComponent;

  UPROPERTY(VisibleAnywhere)
  TObjectPtr<UAuraEquipmentManagerComponent> EquipmentManager;

 private:

  void InputAbilityPressed(FGameplayTag InputTag);

  void InputAbilityReleased(FGameplayTag InputTag);

  FAuraAbilitySet_GrantedHandles AbilitySetHandles;

};