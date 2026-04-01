#pragma once

#include "Animation/AnimSequence.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AuraOverlayTypes.generated.h"

UENUM(BlueprintType)
enum class EAuraOverlayType : uint8 { Base, Status, Equipment };

UENUM(BlueprintType)
enum class EAuraOverlayVariant : uint8 { Relax, Ready, Aim };

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

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TObjectPtr<UAnimSequence> Idle;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TObjectPtr<UAnimSequence> Walk;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TObjectPtr<UAnimSequence> Run;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TObjectPtr<UAnimSequence> Sprint;
};

USTRUCT(BlueprintType)
struct FAuraOverlayVariantSet {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FAuraOverlayMovementSet Relax;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FAuraOverlayMovementSet Ready;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FAuraOverlayMovementSet Aim;
};

USTRUCT(BlueprintType)
struct FAuraOverlayStanceSet {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FAuraOverlayVariantSet Stand;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FAuraOverlayVariantSet Crouch;
};