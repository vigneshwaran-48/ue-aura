#pragma once

#include "CoreMinimal.h"
#include "Interaction/AuraInteractableBase.h"
#include "PickupInteractableBase.generated.h"

class UStaticMeshComponent;
class UAuraItemDefinition;
class UAnimMontage;

UENUM(BlueprintType)
enum class EHandType : uint8
{
	None,
	Left,
	Right
};

UCLASS()
class AURA_API APickupInteractableBase : public AAuraInteractableBase
{
	GENERATED_BODY()

public:
	APickupInteractableBase();

	virtual USceneComponent* GetIndicatorTargetComponent_Implementation() const override;
	virtual void OnInteract(AActor* Interactor) override;

	UFUNCTION(BlueprintCallable, Category = "Interaction|IK")
	FTransform GetHandIKTransform(EHandType HandType, const AActor* Interactor) const;

	UFUNCTION(BlueprintCallable, Category = "Interaction|Pickup")
	void AttachToHand(AActor* Interactor, EHandType HandType);

	UFUNCTION(BlueprintCallable, Category = "Interaction|Pickup")
	void CompletePickupAndAddToInventory(AActor* Interactor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> PickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UAuraItemDefinition> ItemDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Animation")
	TObjectPtr<UAnimMontage> LeftHandPickupMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Animation")
	TObjectPtr<UAnimMontage> RightHandPickupMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|IK")
	FName LeftHandSocketName = FName("Socket_Hand_L");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|IK")
	FName RightHandSocketName = FName("Socket_Hand_R");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|IK")
	FName LeftHandCharacterSocket = FName("hand_l_socket");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|IK")
	FName RightHandCharacterSocket = FName("hand_r_socket");
};