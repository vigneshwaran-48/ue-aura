#pragma once

#include "AuraInteractionOption.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "AuraInteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAuraOnInteractSignature, AActor*,
                                            Interactor);

UCLASS(ClassGroup = (Aura), meta = (BlueprintSpawnableComponent))
class AURA_API UAuraInteractableComponent : public UActorComponent {
  GENERATED_BODY()

 public:
  const TArray<FAuraInteractionOption>& GetInteractionOptions() const;

  UPROPERTY(BlueprintAssignable, Category = "Interaction")
  FAuraOnInteractSignature OnInteract;

  void Interact(AActor* Interactor);

 protected:
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TArray<FAuraInteractionOption> InteractionOptions;
};