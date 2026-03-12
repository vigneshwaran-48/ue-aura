#include "UI/AuraPrimaryGameLayout.h"

#include "UI/AuraUITags.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

void UAuraPrimaryGameLayout::NativeConstruct() {
  Super::NativeConstruct();

  LayerMap.Add(FAuraUITags::UI_Layer_Game, GameStack);
  LayerMap.Add(FAuraUITags::UI_Layer_HUD, HUDStack);
  LayerMap.Add(FAuraUITags::UI_Layer_Menu, MenuStack);
  LayerMap.Add(FAuraUITags::UI_Layer_Modal, ModalStack);
}

UCommonActivatableWidget* UAuraPrimaryGameLayout::PushWidgetToLayer(
    FGameplayTag LayerTag, TSubclassOf<UCommonActivatableWidget> WidgetClass) {
  if (!LayerMap.Contains(LayerTag)) {
    return nullptr;
  }

  UCommonActivatableWidgetStack* Stack = LayerMap[LayerTag];

  return Stack->AddWidget(WidgetClass);
}