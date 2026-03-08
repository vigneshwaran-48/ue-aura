#pragma once

#include "CoreMinimal.h"
#include "AuraInteractionContext.generated.h"

class UAuraInteractableComponent;

USTRUCT(BlueprintType)
struct FAuraInteractionContext {
  GENERATED_BODY()

 public:
  UPROPERTY(BlueprintReadOnly)
  TObjectPtr<AActor> InstigatorActor = nullptr;

  UPROPERTY(BlueprintReadOnly)
  TObjectPtr<AActor> TargetActor = nullptr;

  UPROPERTY(BlueprintReadOnly)
  TObjectPtr<UAuraInteractableComponent> InteractableComponent = nullptr;
};