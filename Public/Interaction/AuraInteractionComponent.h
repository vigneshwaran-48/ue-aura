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
  void UpdateInteractionTarget();
  void ExecuteInteraction();
  AActor* GetInteractableActor() const;
  UAuraInteractableComponent* GetInteractableComponent() const;

 protected:
  virtual void BeginPlay() override;

  UPROPERTY()
  TObjectPtr<UAuraInteractableComponent> CurrentInteractable;

  UPROPERTY()
  TObjectPtr<UAuraInteractableComponent> PreviousInteractable;

  UPROPERTY(EditAnywhere, Category = "Interaction")
  float TraceDistance = 500.f;

  UPROPERTY(EditAnywhere, Category = "Interaction")
  float TraceRadius = 30.f;

 private:
  void SetCurrentInteractable(UAuraInteractableComponent* NewInteractable);
};