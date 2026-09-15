#include "Interaction/PickupInteractableBase.h"
#include "Components/StaticMeshComponent.h"
#include "Inventory/AuraInventoryComponent.h"
#include "Inventory/AuraItemDefinition.h"

APickupInteractableBase::APickupInteractableBase()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = PickupMesh;
}

USceneComponent* APickupInteractableBase::GetIndicatorTargetComponent_Implementation() const
{
	return PickupMesh;
}

void APickupInteractableBase::OnInteract(AActor* Interactor)
{
	Super::OnInteract(Interactor);

	if (!Interactor || !ItemDefinition) return;

	UAuraInventoryComponent* InventoryComp = Interactor->FindComponentByClass<UAuraInventoryComponent>();
	if (!InventoryComp) return;

	FAuraItemHandle AddedHandle = InventoryComp->AddItem(ItemDefinition);

	if (AddedHandle.IsValid())
	{
		OnFocusLost(Interactor);
		OnHighlightLost(Interactor);

		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to add %s to inventory: Inventory full or layout reject."), *GetNameSafe(this));
	}
}