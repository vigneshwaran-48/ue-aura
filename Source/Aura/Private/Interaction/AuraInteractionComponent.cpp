#include "Interaction/AuraInteractionComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AuraGameplayTags.h"
#include "CollisionQueryParams.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Interaction/Abilities/AuraGameplayAbility_Interactable.h"
#include "Interaction/AuraInteractableComponent.h"
#include "Interaction/AuraInteractionMessage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

void UAuraInteractionComponent::BeginPlay() {
  Super::BeginPlay();

  GetWorld()->GetTimerManager().SetTimer(
      ScanTimer, this, &UAuraInteractionComponent::ScanInteractables,
      ScanInterval, true);
}

void UAuraInteractionComponent::UpdateCurrentTarget() {
  AActor* OwnerActor = GetOwner();
  if (!OwnerActor) return;

  FVector Start;
  FRotator Rot;

  OwnerActor->GetActorEyesViewPoint(Start, Rot);

  FVector End = Start + Rot.Vector() * TraceDistance;

  TArray<FHitResult> Hits;

  FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceRadius);

  bool bHit = GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity,
                                              TraceChannel, Sphere);

  UAuraInteractableComponent* FoundInteractable = nullptr;

  if (bHit) {
    for (const FHitResult& Hit : Hits) {
      AActor* HitActor = Hit.GetActor();
      if (!HitActor) continue;

      UAuraInteractableComponent* Interactable =
          HitActor->FindComponentByClass<UAuraInteractableComponent>();

      if (Interactable) {
        FoundInteractable = Interactable;
        break;
      }
    }
  }

  SetCurrentInteractable(FoundInteractable);
}

void UAuraInteractionComponent::SetCurrentInteractable(
    UAuraInteractableComponent* NewInteractable) {
  if (CurrentInteractable == NewInteractable) return;

  PreviousInteractable = CurrentInteractable;
  CurrentInteractable = NewInteractable;

  if (PreviousInteractable) {
    PreviousInteractable->OnFocusLost.Broadcast();
  }

  if (CurrentInteractable) {
    CurrentInteractable->OnFocusGained.Broadcast();
  }
  BroadcastInteractionMessage();
}

AActor* UAuraInteractionComponent::GetInteractableActor() const {
  if (!CurrentInteractable) return nullptr;

  return CurrentInteractable->GetOwner();
}

UAuraInteractableComponent*
UAuraInteractionComponent::GetInteractableComponent() const {
  return CurrentInteractable;
}

void UAuraInteractionComponent::ScanInteractables() {
  AActor* OwnerActor = GetOwner();
  if (!OwnerActor) return;

  UWorld* World = GetWorld();
  if (!World) return;

  TArray<FOverlapResult> Overlaps;

  FCollisionShape Sphere = FCollisionShape::MakeSphere(ScanRadius);

  FCollisionQueryParams Params;
  Params.AddIgnoredActor(OwnerActor);

  bool bHit = World->OverlapMultiByObjectType(
      Overlaps, OwnerActor->GetActorLocation(), FQuat::Identity,
      FCollisionObjectQueryParams(ECC_WorldDynamic), Sphere, Params);

  TArray<UAuraInteractableComponent*> FoundInteractables;

  if (bHit) {
    for (const FOverlapResult& Result : Overlaps) {
      UPrimitiveComponent* Primitive = Result.Component.Get();
      if (!Primitive) continue;

      AActor* HitActor = Primitive->GetOwner();
      if (!HitActor) continue;

      UAuraInteractableComponent* Interactable =
          HitActor->FindComponentByClass<UAuraInteractableComponent>();

      if (Interactable) {
        FoundInteractables.Add(Interactable);
      }
    }
  }

  UpdateHighlights(FoundInteractables);

  UpdateCurrentTarget();
}

void UAuraInteractionComponent::UpdateHighlights(
    const TArray<UAuraInteractableComponent*>& NewInteractables) {
  for (UAuraInteractableComponent* Old : NearbyInteractables) {
    if (!NewInteractables.Contains(Old)) {
      Old->OnDisableHighlight.Broadcast();
    }
  }

  for (UAuraInteractableComponent* New : NewInteractables) {
    if (!NearbyInteractables.Contains(New)) {
      New->OnHighlight.Broadcast();
    }
  }

  NearbyInteractables = NewInteractables;
}

void UAuraInteractionComponent::BroadcastInteractionMessage() {
  FAuraInteractionMessage Message;

  if (CurrentInteractable) {
    Message.TargetActor = CurrentInteractable->GetOwner();
    Message.InteractableComponent = CurrentInteractable;
    Message.InteractionOption = CurrentInteractable->GetInteractionOptions()[0];
  }

  UGameplayMessageSubsystem& Subsystem = UGameplayMessageSubsystem::Get(this);

  Subsystem.BroadcastMessage(TAG_Aura_Message_Interaction_TargetChanged,
                             Message);

  UE_LOG(LogTemp, Log, TEXT("Broadcasting interaction message with target: %s"),
         *GetNameSafe(Message.TargetActor));
}