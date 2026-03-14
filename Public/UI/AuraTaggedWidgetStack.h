#pragma once

#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AuraTaggedWidgetStack.generated.h"

UCLASS()
class AURA_API UAuraTaggedWidgetStack : public UCommonActivatableWidgetStack {
  GENERATED_BODY()

 public:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
  FGameplayTag LayerTag;
};