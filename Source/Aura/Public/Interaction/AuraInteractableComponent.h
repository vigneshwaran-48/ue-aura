#pragma once

#include "AuraInteractionOption.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "AuraInteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAuraOnInteractSignature, AActor*,
                                            Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAuraOnEventSignature, AActor*, Interactor);

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

  UFUNCTION(BlueprintCallable, Category = "Interaction")
  bool IsFocused() const { return bIsFocused; }

  UFUNCTION(BlueprintCallable, Category = "Interaction")
  bool IsHighlighted() const { return bIsHighlighted; }

  void HandleFocusGained(AActor* Interactor);

  void HandleFocusLost(AActor* Interactor);

  void HandleHighlightEnabled(AActor* Interactor);

  void HandleHighlightDisabled(AActor* Interactor);

 protected:

  /** Interaction data */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
  TArray<FAuraInteractionOption> InteractionOptions;

private:
	UPROPERTY(VisibleAnywhere, Category = "Interaction|State")
	bool bIsFocused = false;

	UPROPERTY(VisibleAnywhere, Category = "Interaction|State")
	bool bIsHighlighted = false;

};