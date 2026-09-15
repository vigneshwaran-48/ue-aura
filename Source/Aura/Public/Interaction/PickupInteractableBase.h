#pragma once

#include "CoreMinimal.h"
#include "Interaction/AuraInteractableBase.h"
#include "PickupInteractableBase.generated.h"

class UStaticMeshComponent;
class UAuraItemDefinition;

UCLASS()
class AURA_API APickupInteractableBase : public AAuraInteractableBase
{
	GENERATED_BODY()

public:
	APickupInteractableBase();

	virtual void OnInteract(AActor* Interactor) override;

	virtual USceneComponent* GetIndicatorTargetComponent_Implementation() const override;

	FORCEINLINE UStaticMeshComponent* GetPickupMesh() const { return PickupMesh; }
	FORCEINLINE const UAuraItemDefinition* GetItemDefinition() const { return ItemDefinition; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> PickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<const UAuraItemDefinition> ItemDefinition;
};