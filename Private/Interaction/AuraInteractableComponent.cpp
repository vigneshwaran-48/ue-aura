#include "Interaction/AuraInteractableComponent.h"

const TArray<FAuraInteractionOption>&
UAuraInteractableComponent::GetInteractionOptions() const {
  return InteractionOptions;
}