#include "Interaction/AuraInteractableComponent.h"

const TArray<FAuraInteractionOption>&
UAuraInteractableComponent::GetInteractionOptions() const {
  return InteractionOptions;
}

void UAuraInteractableComponent::Interact(AActor* Interactor) {
  OnInteract.Broadcast(GetOwner());
}