#pragma once

#include "AbilitySystem/AuraAbilitySet.h"
#include "AbilitySystemInterface.h"
#include "Equipment/AuraEquipmentInterface.h"
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
class UAuraPawnData;
class UAuraOverlayComponent;

UCLASS()
class AURA_API AAuraPawn : public APawn, public IAbilitySystemInterface, public IAuraEquipmentInterface {
  GENERATED_BODY()

 public:
  AAuraPawn();

 protected:
  virtual void BeginPlay() override;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aura|GAS")
  TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;

  UPROPERTY(VisibleAnywhere)
  TObjectPtr<UAuraInteractionComponent> InteractionComponent;

  UPROPERTY(VisibleAnywhere)
  TObjectPtr<UAuraEquipmentManagerComponent> EquipmentManager;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UAuraOverlayComponent> OverlayComponent;

  UPROPERTY(Transient, VisibleInstanceOnly)
  TWeakObjectPtr<UCommonActivatableWidget> HUDLayoutWidget;

  UPROPERTY(EditDefaultsOnly, Category = "Aura|Interaction")
  TEnumAsByte<ECollisionChannel> InteractionTraceChannel;

  UPROPERTY(EditDefaultsOnly, Category = "Aura")
  TObjectPtr<UAuraPawnData> PawnData;

 public:
  virtual void Tick(float DeltaTime) override;

  virtual void PossessedBy(AController* NewController) override;

  virtual void UnPossessed() override;

  virtual void SetupPlayerInputComponent(
      UInputComponent* PlayerInputComponent) override;

  virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

  virtual USceneComponent* GetEquipmentAttachComponent_Implementation(
      FName SocketName) const override;

 private:
  void InputAbilityPressed(FGameplayTag InputTag);

  void InputAbilityReleased(FGameplayTag InputTag);

  FAuraAbilitySet_GrantedHandles AbilitySetHandles;

  void InitializeFromPawnData();

  void InitializeAbilities();

  void InitializeInput();

  void InitializeUI();
};
