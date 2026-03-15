#pragma once

#include "AuraInteractionOption.h"
#include "CoreMinimal.h"
#include "AuraInteractionMessage.generated.h"

class UAuraInteractableComponent;

USTRUCT(BlueprintType)
struct FAuraInteractionMessage {
  GENERATED_BODY()

  UPROPERTY(BlueprintReadOnly)
  TObjectPtr<AActor> TargetActor = nullptr;

  UPROPERTY(BlueprintReadOnly)
  FAuraInteractionOption InteractionOption;

  UPROPERTY(BlueprintReadOnly)
  TObjectPtr<UAuraInteractableComponent> InteractableComponent = nullptr;
};