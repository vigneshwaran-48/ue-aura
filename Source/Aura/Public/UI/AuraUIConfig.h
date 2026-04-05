#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AuraUIConfig.generated.h"

class UAuraPrimaryGameLayout;

UCLASS(BlueprintType)
class AURA_API UAuraUIConfig : public UDataAsset {
  GENERATED_BODY()

 public:
  UPROPERTY(EditAnywhere, Category = "UI")
  TSubclassOf<UAuraPrimaryGameLayout> PrimaryGameLayoutClass;
};