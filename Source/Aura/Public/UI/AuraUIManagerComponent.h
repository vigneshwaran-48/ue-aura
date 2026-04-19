#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AuraUIManagerComponent.generated.h"

class UAuraUIWidgetConfig;
class UCommonActivatableWidget;

UCLASS(ClassGroup = (UI), meta = (BlueprintSpawnableComponent))
class AURA_API UAuraUIManagerComponent : public UActorComponent {
  GENERATED_BODY()

 public:
  void EnableUI(FGameplayTag Tag);
  void DisableUI(FGameplayTag Tag);
  void ToggleUI(FGameplayTag Tag);

 protected:
  UPROPERTY(EditDefaultsOnly, Category = "UI")
  TObjectPtr<UAuraUIWidgetConfig> WidgetConfig;

 private:
  UPROPERTY()
  TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidget>> ActiveWidgets;

  UCommonActivatableWidget* CreateAndPush(FGameplayTag Tag);
};