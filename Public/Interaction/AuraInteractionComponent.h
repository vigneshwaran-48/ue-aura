#pragma once

#include "AuraInteractionOption.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "AuraInteractionComponent.generated.h"

class UAuraInteractableComponent;

UCLASS(ClassGroup = (Aura), meta = (BlueprintSpawnableComponent))
class AURA_API UAuraInteractionComponent : public UActorComponent {
  GENERATED_BODY()

 public:
  void ExecuteInteraction();
  AActor* GetInteractableActor() const;
  UAuraInteractableComponent* GetInteractableComponent() const;

  void SetInteractionTraceChannel(ECollisionChannel NewTraceChannel) {
    TraceChannel = NewTraceChannel;
  }

 protected:
  virtual void BeginPlay() override;

  UPROPERTY()
  TObjectPtr<UAuraInteractableComponent> CurrentInteractable;

  UPROPERTY()
  TObjectPtr<UAuraInteractableComponent> PreviousInteractable;

  UPROPERTY()
  TArray<TObjectPtr<UAuraInteractableComponent>> NearbyInteractables;

  void SetCurrentInteractable(UAuraInteractableComponent* NewInteractable);

  void UpdateHighlights(
      const TArray<UAuraInteractableComponent*>& NewInteractables);

  void ScanInteractables();

  void UpdateCurrentTarget();

  void BroadcastInteractionMessage();

  UPROPERTY(EditAnywhere, Category = "Interaction")
  float TraceDistance = 500.f;

  UPROPERTY(EditAnywhere, Category = "Interaction")
  float TraceRadius = 30.f;

  UPROPERTY(EditAnywhere, Category = "Interaction")
  float ScanInterval = 0.1f;

  UPROPERTY(EditAnywhere, Category = "Interaction")
  float ScanRadius = 200.f;

  FTimerHandle ScanTimer;

  ECollisionChannel TraceChannel = ECC_Visibility;
};