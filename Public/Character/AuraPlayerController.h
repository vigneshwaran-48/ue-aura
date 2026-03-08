#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UAuraAbilitySystemComponent;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController {
  GENERATED_BODY()

 public:
  AAuraPlayerController(
      const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

  // Called after input is processed each frame
  virtual void PostProcessInput(float DeltaTime, bool bGamePaused) override;

 protected:
  // Helper: get ASC from controlled pawn
  class UAuraAbilitySystemComponent* GetAbilitySystemComponent() const;
};
