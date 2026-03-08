#pragma once

#include "AuraInteractionOption.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "AuraInteractableComponent.generated.h"

UCLASS(ClassGroup = (Aura), meta = (BlueprintSpawnableComponent))
class AURA_API UAuraInteractableComponent : public UActorComponent {
  GENERATED_BODY()

 public:
  const TArray<FAuraInteractionOption>& GetInteractionOptions() const;

 protected:
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TArray<FAuraInteractionOption> InteractionOptions;
};