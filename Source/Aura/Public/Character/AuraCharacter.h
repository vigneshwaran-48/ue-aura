#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Equipment/AuraEquipmentInterface.h"
#include "Character/AuraPawnDataProvider.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AuraAbilitySet.h"
#include "AuraCharacter.generated.h"

class UAuraAbilitySystemComponent;
class UAbilitySystemComponent;
class UAuraEquipmentManagerComponent;
class UAuraInteractionComponent;
class UAuraInventoryComponent;
class UAuraOverlayComponent;
class UAuraUIManagerComponent;
class UAuraPawnData;

UCLASS()
class AURA_API AAuraCharacter : public ACharacter,
                                public IAbilitySystemInterface,
                                public IAuraEquipmentInterface,
                                public IAuraPawnDataProvider 
{
    GENERATED_BODY()

public:
    AAuraCharacter();

    // Actor Lifecycle
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void UnPossessed() override;

    // IAbilitySystemInterface
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    // IAuraEquipmentInterface
    virtual USceneComponent* GetEquipmentAttachComponent_Implementation(FName SocketName) const override;

    // IAuraPawnDataProvider
    virtual const UAuraPawnData* GetPawnData_Implementation() const override;

protected:
    // Core GAS Component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aura|GAS")
    TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;

    // Modular Subsystems / Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aura|Components")
    TObjectPtr<UAuraInteractionComponent> InteractionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aura|Components")
    TObjectPtr<UAuraEquipmentManagerComponent> EquipmentManager;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aura|Components")
    TObjectPtr<UAuraInventoryComponent> InventoryComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aura|Components")
    TObjectPtr<UAuraOverlayComponent> OverlayComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aura|Components")
    TObjectPtr<UAuraUIManagerComponent> UIManagerComponent;

    // Unified Lyra-style Initialization Data Asset
    UPROPERTY(EditDefaultsOnly, Category = "Aura|Data")
    TObjectPtr<UAuraPawnData> PawnData;

private:
    // Enhanced Input & Ability Binding Callbacks
    void InputAbilityPressed(FGameplayTag InputTag);
    void InputAbilityReleased(FGameplayTag InputTag);

    // Initialization Pipeline Helpers
    void InitializeFromPawnData();
    void InitializeAbilities();

    // Tracks handles granted by the data asset's ability sets
    FAuraAbilitySet_GrantedHandles AbilitySetHandles;
};
