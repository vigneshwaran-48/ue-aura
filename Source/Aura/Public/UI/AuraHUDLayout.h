#pragma once

#include "CoreMinimal.h"
#include "UI/AuraActivatableWidget.h"
#include "AuraHUDLayout.generated.h"

UCLASS()
class AURA_API UAuraHUDLayout : public UAuraActivatableWidget {
  GENERATED_BODY()

 public:
  // Set Class Defaults
  UAuraHUDLayout(
      const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

  //~Begin UUserWidget interface
  virtual void NativeConstruct() override;
  virtual void NativeDestruct() override;
  //~End UUserWidget interface

 protected:
  /** Callback called whenever the player gives the "Show Main Menu" input
   */
  void HandleMainMenuAction();

  /** Class to use as the Main Menu for this HUD Layout */
  UPROPERTY(EditDefaultsOnly)
  TSoftClassPtr<UAuraActivatableWidget> MainMenuClass;

 private:
  FUIActionBindingHandle MenuActionHandle;
};
