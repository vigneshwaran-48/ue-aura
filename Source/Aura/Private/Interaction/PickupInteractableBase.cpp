#include "Interaction/PickupInteractableBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Inventory/AuraInventoryComponent.h"
#include "Inventory/AuraItemDefinition.h"
#include "Interaction/Animation/AuraInteractionAnimInstance.h"
#include "Equipment/AuraEquipmentManagerComponent.h"
#include "Equipment/Fragments/AuraItemFragment_EquippableItem.h"
#include "GameFramework/Pawn.h"

APickupInteractableBase::APickupInteractableBase()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = PickupMesh;
}

USceneComponent* APickupInteractableBase::GetIndicatorTargetComponent_Implementation() const
{
	return PickupMesh;
}

FTransform APickupInteractableBase::GetHandIKTransform(EHandType HandType, const AActor* Interactor) const
{
	FName SocketName = (HandType == EHandType::Left) ? LeftHandSocketName : RightHandSocketName;

	if (PickupMesh && PickupMesh->DoesSocketExist(SocketName))
	{
		return PickupMesh->GetSocketTransform(SocketName, RTS_World);
	}

	return GetActorTransform();
}

void APickupInteractableBase::OnInteract(AActor* Interactor)
{
	Super::OnInteract(Interactor);

	if (!Interactor || !ItemDefinition) return;

	UAuraInventoryComponent* InventoryComp = Interactor->FindComponentByClass<UAuraInventoryComponent>();
	if (!InventoryComp || !InventoryComp->CanAddItem(ItemDefinition))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot pick up %s: Inventory is full or has no fitting slot."), *GetNameSafe(this));
		return;
	}

	APawn* Pawn = Cast<APawn>(Interactor);
	if (!Pawn) return;

	USkeletalMeshComponent* MeshComp = Pawn->FindComponentByClass<USkeletalMeshComponent>();
	if (!MeshComp) return;

	UAnimInstance* MainAnimInstance = MeshComp->GetAnimInstance();
	if (!MainAnimInstance) return;

	UAuraInteractionAnimInstance* InteractionAnimInst = Cast<UAuraInteractionAnimInstance>(
		MainAnimInstance->GetLinkedAnimGraphInstanceByTag(FName("InteractionLayer"))
	);

	if (!InteractionAnimInst)
	{
		InteractionAnimInst = Cast<UAuraInteractionAnimInstance>(MainAnimInstance);
	}

	EHandType SelectedHand = EHandType::Right;

	if (InteractionAnimInst)
	{
		InteractionAnimInst->SetupPickupIK(this);
		SelectedHand = InteractionAnimInst->GetActiveHand();
	}
	else
	{
		FVector DirToTarget = (GetActorLocation() - Pawn->GetActorLocation()).GetSafeNormal2D();
		float RightDot = FVector::DotProduct(Pawn->GetActorRightVector(), DirToTarget);
		SelectedHand = (RightDot >= 0.0f) ? EHandType::Right : EHandType::Left;
	}

	UAnimMontage* TargetMontage = (SelectedHand == EHandType::Left) ? LeftHandPickupMontage : RightHandPickupMontage;

	if (TargetMontage)
	{
		MainAnimInstance->Montage_Play(TargetMontage);
	}
}

void APickupInteractableBase::AttachToHand(AActor* Interactor, EHandType HandType)
{
	APawn* Pawn = Cast<APawn>(Interactor);
	if (!Pawn) return;

	USkeletalMeshComponent* MeshComp = Pawn->FindComponentByClass<USkeletalMeshComponent>();
	if (!MeshComp) return;

	FName HandSocket = (HandType == EHandType::Left) ? LeftHandCharacterSocket : RightHandCharacterSocket;

	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupMesh->SetSimulatePhysics(false);

	AttachToComponent(
		MeshComp,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		HandSocket
	);
}

void APickupInteractableBase::CompletePickupAndAddToInventory(AActor* Interactor)
{
	if (!Interactor || !ItemDefinition) return;

	UAuraInventoryComponent* InventoryComp = Interactor->FindComponentByClass<UAuraInventoryComponent>();
	if (!InventoryComp) return;

	FAuraItemHandle AddedHandle = InventoryComp->AddItem(ItemDefinition);

	if (AddedHandle.IsValid())
	{

		UAuraEquipmentManagerComponent* EquipmentManager = Interactor->FindComponentByClass<UAuraEquipmentManagerComponent>();

		const UAuraItemFragment_EquippableItem* EquipmentDefinitionFrag = ItemDefinition->FindFragment<UAuraItemFragment_EquippableItem>();
		if (EquipmentManager && EquipmentDefinitionFrag) {
			EquipmentManager->EquipItem(EquipmentDefinitionFrag->EquipmentDefinition);
		}
		OnFocusLost(Interactor);
		OnHighlightLost(Interactor);
		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to add %s to inventory: Inventory full or layout reject."), *GetNameSafe(this));

		// If inventory fails, detach and restore physics in world space
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PickupMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}