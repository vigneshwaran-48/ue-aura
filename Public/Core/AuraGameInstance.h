#pragma once

#include "CoreMinimal.h"
#include "CommonGameInstance.h"
#include "Engine/GameInstance.h"
#include "AuraGameInstance.generated.h"

class UAuraUIConfig;

UCLASS()
class AURA_API UAuraGameInstance : public UCommonGameInstance {
  GENERATED_BODY()

 public:
  UAuraGameInstance(
      const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

  UPROPERTY(EditDefaultsOnly, Category = "UI")
  TObjectPtr<UAuraUIConfig> UIConfig;
};