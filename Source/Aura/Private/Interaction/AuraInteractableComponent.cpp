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

void UAuraInteractableComponent::HandleFocusGained(AActor* Interactor) const {
  OnFocusGained.Broadcast(Interactor);
}

void UAuraInteractableComponent::HandleFocusLost(AActor* Interactor) const {
	OnFocusLost.Broadcast(Interactor);
}

void UAuraInteractableComponent::HandleHighlightDisabled(AActor* Interactor) const {
	OnDisableHighlight.Broadcast(Interactor);
}

void UAuraInteractableComponent::HandleHighlightEnabled(AActor* Interactor) const {
	OnHighlight.Broadcast(Interactor);
}