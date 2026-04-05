#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/AuraUILayoutEntry.h"
#include "CommonPlayerController.h"
#include "AuraPlayerController.generated.h"

class UAuraAbilitySystemComponent;

UCLASS()
class AURA_API AAuraPlayerController : public ACommonPlayerController {
  GENERATED_BODY()

 public:
  AAuraPlayerController(
      const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

 protected:
  // Helper: get ASC from controlled pawn
  class UAuraAbilitySystemComponent* GetAbilitySystemComponent() const;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
  TArray<FAuraUILayoutEntry> InitialUILayouts;

  void BeginPlay() override;
};
