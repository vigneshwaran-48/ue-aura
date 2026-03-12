#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "AuraUISubsystem.generated.h"

class UAuraUIPolicy;
class UAuraPrimaryGameLayout;

UCLASS()
class AURA_API UAuraUISubsystem : public UGameInstanceSubsystem {
  GENERATED_BODY()

 public:
  void InitializeUI(APlayerController* PC);

  UAuraPrimaryGameLayout* GetRootLayout() const;

 private:
  UPROPERTY()
  UAuraUIPolicy* UIPolicy;
};