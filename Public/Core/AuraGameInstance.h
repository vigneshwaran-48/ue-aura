#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuraGameInstance.generated.h"

class UAuraUIConfig;

UCLASS()
class AURA_API UAuraGameInstance : public UGameInstance {
  GENERATED_BODY()

 public:
  UPROPERTY(EditDefaultsOnly, Category = "UI")
  TObjectPtr<UAuraUIConfig> UIConfig;
};