#pragma once

#include "AuraInteractionOption.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "AuraInteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAuraOnInteractSignature, AActor*,
                                            Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAuraOnEventSignature);

UCLASS(ClassGroup = (Aura), meta = (BlueprintSpawnableComponent))
class AURA_API UAuraInteractableComponent : public UActorComponent {
  GENERATED_BODY()

 public:
  UAuraInteractableComponent();

  const TArray<FAuraInteractionOption>& GetInteractionOptions() const;

  UPROPERTY(BlueprintAssignable, Category = "Interaction")
  FAuraOnInteractSignature OnInteract;

  void Interact(AActor* Interactor) const;

  UPROPERTY(BlueprintAssignable, Category = "Interaction")
  FAuraOnEventSignature OnHighlight;

  UPROPERTY(BlueprintAssignable, Category = "Interaction")
  FAuraOnEventSignature OnDisableHighlight;

  UPROPERTY(BlueprintAssignable, Category = "Interaction")
  FAuraOnEventSignature OnFocusGained;

  UPROPERTY(BlueprintAssignable, Category = "Interaction")
  FAuraOnEventSignature OnFocusLost;

  void HandleFocusGained() const;

  void HandleFocusLost() const;

 protected:

  /** Interaction data */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
  TArray<FAuraInteractionOption> InteractionOptions;

};