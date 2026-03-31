#pragma once

#include "Animation/AnimSequence.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AuraOverlayTypes.generated.h"

UENUM(BlueprintType)
enum class EAuraOverlayType : uint8 { Base, Status, Equipment };

USTRUCT(BlueprintType)
struct FAuraOverlaySlot {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FGameplayTag SlotTag;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  float Weight = 1.f;
};

USTRUCT(BlueprintType)
struct FAuraOverlayMovementSet {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere)
  TObjectPtr<UAnimSequence> Idle;

  UPROPERTY(EditAnywhere)
  TObjectPtr<UAnimSequence> Walk;

  UPROPERTY(EditAnywhere)
  TObjectPtr<UAnimSequence> Run;

  UPROPERTY(EditAnywhere)
  TObjectPtr<UAnimSequence> Sprint;
};

USTRUCT(BlueprintType)
struct FAuraOverlayVariantSet {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere)
  FAuraOverlayMovementSet Relax;

  UPROPERTY(EditAnywhere)
  FAuraOverlayMovementSet Ready;

  UPROPERTY(EditAnywhere)
  FAuraOverlayMovementSet Aim;
};

USTRUCT(BlueprintType)
struct FAuraOverlayStanceSet {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere)
  FAuraOverlayVariantSet Stand;

  UPROPERTY(EditAnywhere)
  FAuraOverlayVariantSet Crouch;
};