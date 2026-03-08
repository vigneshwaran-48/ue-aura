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
  int32 MashTarget = 5;

  UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Interaction|Mash",
      meta = (EditCondition = "InteractionType == EAuraInteractionType::Mash",
              EditConditionHides))
  float MashTimeLimit = 3.f;
};