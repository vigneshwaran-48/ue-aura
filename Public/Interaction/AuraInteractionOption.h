#pragma once

#include "CoreMinimal.h"
#include "Interaction/AuraInteractionType.h"
#include "AuraInteractionOption.generated.h"

USTRUCT(BlueprintType)
struct FAuraInteractionOption {
  GENERATED_BODY()

 public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
  EAuraInteractionType InteractionType = EAuraInteractionType::Instant;

  /* ---------------- Hold ---------------- */

  UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Interaction|Hold",
      meta = (EditCondition = "InteractionType == EAuraInteractionType::Hold",
              EditConditionHides))
  float HoldDuration = 2.f;

  /* ---------------- Mash ---------------- */

  UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Interaction|Mash",
      meta = (EditCondition = "InteractionType == EAuraInteractionType::Mash",
              EditConditionHides))
  float MashIncrement = 10.f;

  UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Interaction|Mash",
      meta = (EditCondition = "InteractionType == EAuraInteractionType::Mash",
              EditConditionHides))
  float MashDecayRate = 25.f;

  UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Interaction|Mash",
      meta = (EditCondition = "InteractionType == EAuraInteractionType::Mash",
              EditConditionHides))
  float MashTargetProgress = 100.f;
};