#include "Interaction/AuraInteractableComponent.h"

#include "Blueprint/UserWidget.h"

UAuraInteractableComponent::UAuraInteractableComponent() {
  PrimaryComponentTick.bCanEverTick = false;
}

const TArray<FAuraInteractionOption>&
UAuraInteractableComponent::GetInteractionOptions() const {
  return InteractionOptions;
}

void UAuraInteractableComponent::Interact(AActor* Interactor) {
  OnInteract.Broadcast(GetOwner());
}

void UAuraInteractableComponent::HandleFocusGained() {
  OnFocusGained.Broadcast();
}

void UAuraInteractableComponent::HandleFocusLost() {
  OnFocusLost.Broadcast();
}