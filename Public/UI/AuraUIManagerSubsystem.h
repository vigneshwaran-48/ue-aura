#pragma once

#include "Containers/Ticker.h"
#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "AuraUIManagerSubsystem.generated.h"

UCLASS()
class AURA_API UAuraUIManagerSubsystem : public UGameUIManagerSubsystem {
  GENERATED_BODY()

 public:
  UAuraUIManagerSubsystem();

  //~USubsystem interface
  virtual void Initialize(FSubsystemCollectionBase& Collection) override;
  virtual void Deinitialize() override;
  //~End of USubsystem interface

 private:
  bool Tick(float DeltaTime);
  void SyncRootLayoutVisibilityToShowHUD();

  FTSTicker::FDelegateHandle TickHandle;
};
