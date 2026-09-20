#include "Interaction/Animation/AuraInteractionAnimInstance.h"
#include "GameFramework/Pawn.h"

void UAuraInteractionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	BodyAimAlpha = GetCurveValue(BodyAimCurveName);
	LeftHandIKAlpha = GetCurveValue(LeftHandIKCurveName);
	RightHandIKAlpha = GetCurveValue(RightHandIKCurveName);
}

void UAuraInteractionAnimInstance::SetupPickupIK(APickupInteractableBase* PickupTarget)
{
	CurrentPickupTarget = PickupTarget;
	if (!CurrentPickupTarget) return;

	APawn* Pawn = Cast<APawn>(GetOwningActor());
	if (!Pawn) return;

	FVector DirToTarget = (CurrentPickupTarget->GetActorLocation() - Pawn->GetActorLocation()).GetSafeNormal2D();
	float RightDot = FVector::DotProduct(Pawn->GetActorRightVector(), DirToTarget);

	ActiveHand = (RightDot >= 0.0f) ? EHandType::Right : EHandType::Left;

	FTransform TargetTransform = CurrentPickupTarget->GetHandIKTransform(ActiveHand, Pawn);

	if (ActiveHand == EHandType::Left)
	{
		LeftHandIKTarget = TargetTransform;
	}
	else
	{
		RightHandIKTarget = TargetTransform;
	}

	BodyAimTargetLocation = CurrentPickupTarget->GetActorLocation();
}