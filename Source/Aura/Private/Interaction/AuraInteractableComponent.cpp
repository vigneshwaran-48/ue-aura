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

void UAuraInteractableComponent::HandleFocusGained(AActor* Interactor) {
  bIsFocused = true;
  OnFocusGained.Broadcast(Interactor);
}

void UAuraInteractableComponent::HandleFocusLost(AActor* Interactor) {
	bIsFocused = false;
	OnFocusLost.Broadcast(Interactor);
}

void UAuraInteractableComponent::HandleHighlightDisabled(AActor* Interactor) {
	bIsHighlighted = false;
	OnDisableHighlight.Broadcast(Interactor);
}

void UAuraInteractableComponent::HandleHighlightEnabled(AActor* Interactor) {
	bIsHighlighted = true;
	OnHighlight.Broadcast(Interactor);
}