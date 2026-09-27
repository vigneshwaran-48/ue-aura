#include "UI/AuraGameViewportClient.h"

#include "CommonUISettings.h"
#include "ICommonUIModule.h"
#include "Framework/Application/NavigationConfig.h"

namespace GameViewportTags {
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Platform_Trait_Input_HardwareCursor,
                              "Platform.Trait.Input.HardwareCursor");
}

UAuraGameViewportClient::UAuraGameViewportClient(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {}

void UAuraGameViewportClient::Init(FWorldContext& WorldContext,
                                   UGameInstance* OwningGameInstance,
                                   bool bCreateNewAudioDevice) {
  Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);

  TSharedRef<FNavigationConfig> SlateNavConfig = FSlateApplication::Get().GetNavigationConfig();

  // WARNING: If in future there is any issue where a widget is not receiving arrow key input for unreal engine widgets like sliders, check this code first.
  // Clear out the default key maps so Slate stops stealing arrow keys from CommonUI
  // This empties out the rules keeping track of Left, Right, Up, Down arrow keys
  SlateNavConfig->KeyEventRules.Empty();
  // Clear out the default analog/d-pad overrides if you want custom gamepad mappings to work reliably
  // Set them to unmapped virtual keys (or EKeys::Invalid)
  SlateNavConfig->AnalogHorizontalKey = EKeys::Invalid;
  SlateNavConfig->AnalogVerticalKey = EKeys::Invalid;
  FSlateApplication::Get().SetNavigationConfig(SlateNavConfig);

  // We have software cursors set up in our project settings for console/mobile
  // use, but on desktop native hardware cursors are preferred.
  const bool bUseHardwareCursor =
      ICommonUIModule::GetSettings().GetPlatformTraits().HasTag(
          GameViewportTags::TAG_Platform_Trait_Input_HardwareCursor);
  SetUseSoftwareCursorWidgets(!bUseHardwareCursor);
}
