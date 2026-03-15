#include "Interaction/AuraInteractableComponent.h"

UAuraInteractableComponent::UAuraInteractableComponent() {
  PrimaryComponentTick.bCanEverTick = false;
}

void UAuraInteractableComponent::BeginPlay() {
  Super::BeginPlay();
}

const TArray<FAuraInteractionOption>&
UAuraInteractableComponent::GetInteractionOptions() const {
  return InteractionOptions;
}

void UAuraInteractableComponent::Interact(AActor* Interactor) {
  OnInteract.Broadcast(GetOwner());
}
