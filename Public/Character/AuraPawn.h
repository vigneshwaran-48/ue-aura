#pragma once

#include "AbilitySystem/AuraAbilitySet.h"
#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameplayTagContainer.h"
#include "AuraPawn.generated.h"

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
class AURA_API AAuraPawn : public APawn, public IAbilitySystemInterface {
  GENERATED_BODY()

 public:
  AAuraPawn();

 protected:
  virtual void BeginPlay() override;

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

 public:
  virtual void Tick(float DeltaTime) override;

  virtual void PossessedBy(AController* NewController) override;

  virtual void UnPossessed() override;

  virtual void SetupPlayerInputComponent(
      UInputComponent* PlayerInputComponent) override;

  virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

 private:
  void InputAbilityPressed(FGameplayTag InputTag);

  void InputAbilityReleased(FGameplayTag InputTag);

  FAuraAbilitySet_GrantedHandles AbilitySetHandles;
};
