#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "AuraUISubsystem.generated.h"

class UAuraUIPolicy;
class UAuraPrimaryGameLayout;
class UCommonActivatableWidget;

struct FAuraUIPushRequest {
  FGameplayTag LayerTag;

  TSubclassOf<UCommonActivatableWidget> WidgetClass;
};

UCLASS()
class AURA_API UAuraUISubsystem : public UGameInstanceSubsystem {
  GENERATED_BODY()

 public:
  virtual void Initialize(FSubsystemCollectionBase& Collection) override;

  void InitializeUI(APlayerController* PC);

  UAuraPrimaryGameLayout* GetRootLayout() const;

  UCommonActivatableWidget* PushWidgetToLayer(
      FGameplayTag LayerTag, TSubclassOf<UCommonActivatableWidget> WidgetClass);

 private:
  UPROPERTY()
  UAuraUIPolicy* UIPolicy;

  FTimerHandle UIInitTimerHandle;

  TArray<FAuraUIPushRequest> PendingPushRequests;

  UFUNCTION()
  void TryInitializeUI();
};