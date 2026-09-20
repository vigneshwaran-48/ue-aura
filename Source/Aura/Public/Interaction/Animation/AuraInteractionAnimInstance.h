// UAuraInteractionAnimInstance.h
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interaction/PickupInteractableBase.h"
#include "AuraInteractionAnimInstance.generated.h"

UCLASS()
class AURA_API UAuraInteractionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Interaction|IK")
	void SetupPickupIK(APickupInteractableBase* PickupTarget);

	UFUNCTION(BlueprintCallable, Category = "Interaction|IK")
	EHandType GetActiveHand() const { return ActiveHand; }

	UFUNCTION(BlueprintCallable, Category = "Interaction|IK")
	APickupInteractableBase* GetCurrentPickupTarget() const { return CurrentPickupTarget; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|IK|Curves")
	FName BodyAimCurveName = FName("IK_Interaction_BodyAim");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|IK|Curves")
	FName LeftHandIKCurveName = FName("IK_Interaction_LeftHand");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|IK|Curves")
	FName RightHandIKCurveName = FName("IK_Interaction_RightHand");



	UPROPERTY(BlueprintReadOnly, Category = "Interaction|IK|Alphas")
	float BodyAimAlpha = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction|IK|Alphas")
	float LeftHandIKAlpha = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction|IK|Alphas")
	float RightHandIKAlpha = 0.0f;



	UPROPERTY(BlueprintReadOnly, Category = "Interaction|IK")
	FTransform LeftHandIKTarget;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction|IK")
	FTransform RightHandIKTarget;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction|IK")
	FVector BodyAimTargetLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction|IK")
	EHandType ActiveHand = EHandType::None;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction|IK")
	TObjectPtr<APickupInteractableBase> CurrentPickupTarget;
};