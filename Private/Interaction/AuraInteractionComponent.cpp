#include "Interaction/AuraInteractionComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Interaction/Abilities/AuraGameplayAbility_Interactable.h"
#include "Interaction/AuraInteractableComponent.h"

void UAuraInteractionComponent::BeginPlay() { Super::BeginPlay(); }

void UAuraInteractionComponent::UpdateInteractionTarget() {
  AActor* OwnerActor = GetOwner();
  if (!OwnerActor) return;

  FVector Start;
  FRotator Rot;

  OwnerActor->GetActorEyesViewPoint(Start, Rot);

  FVector End = Start + Rot.Vector() * TraceDistance;

  TArray<FHitResult> Hits;

  FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceRadius);

  bool bHit = GetWorld()->SweepMultiByChannel(
      Hits, Start, End, FQuat::Identity,
      /* Interactable Channel */ ECC_GameTraceChannel1, Sphere);

  UAuraInteractableComponent* FoundInteractable = nullptr;

  if (bHit) {
    for (const FHitResult& Hit : Hits) {
      if (!Hit.GetActor()) continue;

      UAuraInteractableComponent* Interactable =
          Hit.GetActor()->FindComponentByClass<UAuraInteractableComponent>();

      if (Interactable) {
#if WITH_EDITOR
        DrawDebugSphere(GetWorld(), End, TraceRadius, 16, FColor::Green, false,
                        0.1f);
#endif
        FoundInteractable = Interactable;
        break;
      }
    }
  }

  SetCurrentInteractable(FoundInteractable);
}

void UAuraInteractionComponent::ExecuteInteraction() {
  if (!CurrentInteractable) {
    UE_LOG(LogTemp, Warning, TEXT("No interactable found!"));
    return;
  }


}

void UAuraInteractionComponent::SetCurrentInteractable(
    UAuraInteractableComponent* NewInteractable) {
  if (CurrentInteractable == NewInteractable) return;

  PreviousInteractable = CurrentInteractable;
  CurrentInteractable = NewInteractable;

  if (PreviousInteractable) {
    // TODO: OnFocusLost
  }

  if (CurrentInteractable) {
    // TODO: OnFocusGained
  }
}

AActor* UAuraInteractionComponent::GetInteractableActor() const {
  if (!CurrentInteractable) return nullptr;

  return CurrentInteractable->GetOwner();
}

UAuraInteractableComponent*
UAuraInteractionComponent::GetInteractableComponent() const {
  return CurrentInteractable;
}