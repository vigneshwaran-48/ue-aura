#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AuraUIWidgetConfig.generated.h"

class UCommonActivatableWidget;

USTRUCT(BlueprintType)
struct FAuraUIWidgetEntry {
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly)
  FGameplayTag Tag;

  UPROPERTY(EditDefaultsOnly)
  FGameplayTag LayerTag;

  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<UCommonActivatableWidget> WidgetClass;
};

UCLASS(BlueprintType)
class AURA_API UAuraUIWidgetConfig : public UDataAsset {
  GENERATED_BODY()

 public:
  UPROPERTY(EditDefaultsOnly)
  TArray<FAuraUIWidgetEntry> Widgets;

  const FAuraUIWidgetEntry* FindEntry(FGameplayTag Tag) const {
    for (const FAuraUIWidgetEntry& Entry : Widgets) {
      if (Entry.Tag.MatchesTagExact(Tag)) {
        return &Entry;
      }
    }
    return nullptr;
  }
};