#include "Interaction/Animation/AN_CompletePickup.h"
#include "Interaction/Animation/AuraInteractionAnimInstance.h"

void UAN_CompletePickup::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

	APawn* Pawn = Cast<APawn>(MeshComp->GetOwner());
	if (!Pawn) return;

	UAuraInteractionAnimInstance* AnimInst = Cast<UAuraInteractionAnimInstance>(
		MeshComp->GetAnimInstance()->GetLinkedAnimGraphInstanceByTag(FName("InteractionLayer"))
	);

	if (!AnimInst)
	{
		AnimInst = Cast<UAuraInteractionAnimInstance>(MeshComp->GetAnimInstance());
	}

	if (AnimInst && AnimInst->GetCurrentPickupTarget())
	{
		AnimInst->GetCurrentPickupTarget()->CompletePickupAndAddToInventory(Pawn);
	}
}