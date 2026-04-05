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
class UCommonActivatableWidget;
class UAuraHUDLayout;

UCLASS()
class AURA_API AAuraCharacter : public ACharacter,
                                public IAbilitySystemInterface {
  GENERATED_BODY()

 public:
  AAuraCharacter();

  virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

 protected:
  virtual void BeginPlay() override;

  virtual void PossessedBy(AController* NewController) override;

  virtual void UnPossessed() override;

  virtual void SetupPlayerInputComponent(
      UInputComponent* PlayerInputComponent) override;

  virtual void Tick(float DeltaTime) override;

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

  /** The HUD Layout widget to use (must be derived from Aura HUD Layout) */
  UPROPERTY(EditDefaultsOnly, DisplayName = "Aura|HUD Layout")
  TSubclassOf<UAuraHUDLayout> HUDLayoutClass;

  /** Used to keep track of the widget that was created to be our HUD */
  UPROPERTY(Transient, VisibleInstanceOnly)
  TWeakObjectPtr<UCommonActivatableWidget> HUDLayoutWidget;

  UPROPERTY(EditDefaultsOnly, DisplayName = "Aura|Interaction")
  TEnumAsByte<ECollisionChannel> InteractionTraceChannel;

 private:

  void InputAbilityPressed(FGameplayTag InputTag);

  void InputAbilityReleased(FGameplayTag InputTag);

  FAuraAbilitySet_GrantedHandles AbilitySetHandles;

};