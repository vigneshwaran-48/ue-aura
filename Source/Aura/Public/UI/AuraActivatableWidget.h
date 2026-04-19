#pragma once

#include "CommonActivatableWidget.h"
#include "AuraActivatableWidget.generated.h"

/**
 * EAuraWidgetInputMode
 *
 * Input Modes that AuraActivatableWidgets can activate
 */
UENUM(BlueprintType)
enum class EAuraWidgetInputMode : uint8 { Default, GameAndMenu, Game, Menu };

UCLASS()
class AURA_API UAuraActivatableWidget : public UCommonActivatableWidget {
  GENERATED_BODY()

 public:
  // Set Class Defaults
  UAuraActivatableWidget(
      const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

  //~Begin UCommonActivatableWidget interface
  virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
  //~End UCommonActivatableWidget interface

  /** @return the ViewportClient of this widget's owning player */
  UGameViewportClient* GetViewportClient() const;

 protected:
  /**
   * The desired input mode to use while this UI is activated
   *
   * When EAuraWidgetInputMode::Default, NO CHANGES are made to input settings
   * upon Activation of this widget.
   */
  UPROPERTY(EditDefaultsOnly, Category = Input)
  EAuraWidgetInputMode InputMode = EAuraWidgetInputMode::Default;

  UPROPERTY(EditDefaultsOnly, Category = Input)
  EMouseCaptureMode GameMouseCaptureMode =
      EMouseCaptureMode::CapturePermanently;
};