#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AuraUISettings.generated.h"

class UAuraPrimaryGameLayout;

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Aura UI"))
class AURA_API UAuraUISettings : public UDeveloperSettings {
  GENERATED_BODY()

 public:
  UAuraUISettings();

  UPROPERTY(EditAnywhere, Config, Category = "UI")
  TSubclassOf<UAuraPrimaryGameLayout> PrimaryGameLayoutClass;

  virtual FName GetCategoryName() const override { return TEXT("Aura"); }
};