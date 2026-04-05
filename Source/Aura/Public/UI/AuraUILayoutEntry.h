#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CommonActivatableWidget.h"
#include "AuraUILayoutEntry.generated.h"

class UCommonActivatableWidget;

USTRUCT(BlueprintType)
struct FAuraUILayoutEntry {
  GENERATED_BODY()

 public:
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FGameplayTag LayerTag;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TSubclassOf<UCommonActivatableWidget> WidgetClass;
};