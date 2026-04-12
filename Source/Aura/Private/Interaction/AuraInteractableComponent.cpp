#include "Interaction/AuraInteractableComponent.h"

#include "Blueprint/UserWidget.h"

UAuraInteractableComponent::UAuraInteractableComponent() {
  PrimaryComponentTick.bCanEverTick = false;
}

const TArray<FAuraInteractionOption>&
UAuraInteractableComponent::GetInteractionOptions() const {
  return InteractionOptions;
}

void UAuraInteractableComponent::Interact(AActor* Interactor) const {
  OnInteract.Broadcast(Interactor);
}

void UAuraInteractableComponent::HandleFocusGained() const {
  OnFocusGained.Broadcast();
}

void UAuraInteractableComponent::HandleFocusLost() const {
  OnFocusLost.Broadcast();
}