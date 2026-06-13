#pragma once

#include "CommonPlayerController.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/AuraUILayoutEntry.h"
#include "AuraPlayerController.generated.h"

class UAuraAbilitySystemComponent;
class UInputMappingContext;
class UCommonActivatableWidget;
class UAuraPawnData;

UCLASS()
class AURA_API AAuraPlayerController : public ACommonPlayerController {
  GENERATED_BODY()

public:
  AAuraPlayerController(
      const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

protected:
  // Helper: get ASC from controlled pawn
  class UAuraAbilitySystemComponent *GetAbilitySystemComponent() const;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
  TArray<FAuraUILayoutEntry> InitialUILayouts;

  void BeginPlay() override;

  virtual void OnPossess(APawn *InPawn) override;

  virtual void OnUnPossess() override;

  virtual void SetPawn(APawn* InPawn) override;

private:
  void ApplyPawnData(const UAuraPawnData *PawnData);
  void RemoveCurrentPawnPresentation();

private:
  UPROPERTY()
  TObjectPtr<UInputMappingContext> ActiveInputMappingContext;

  UPROPERTY()
  TWeakObjectPtr<UCommonActivatableWidget> ActiveHUDLayoutWidget;
};
