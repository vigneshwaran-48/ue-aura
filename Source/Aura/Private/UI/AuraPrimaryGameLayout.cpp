#include "UI/AuraPrimaryGameLayout.h"

#include "Blueprint/WidgetTree.h"
#include "UI/AuraTaggedWidgetStack.h"

void UAuraPrimaryGameLayout::NativeConstruct() {
  Super::NativeConstruct();

  LayerMap.Empty();

  TArray<UWidget*> AllWidgets;
  WidgetTree->GetAllWidgets(AllWidgets);

  for (UWidget* Widget : AllWidgets) {
    UAuraTaggedWidgetStack* Stack = Cast<UAuraTaggedWidgetStack>(Widget);

    if (Stack && Stack->LayerTag.IsValid()) {
      LayerMap.Add(Stack->LayerTag, Stack);
    }
  }
}

UCommonActivatableWidget* UAuraPrimaryGameLayout::PushWidgetToLayer(
    FGameplayTag LayerTag, TSubclassOf<UCommonActivatableWidget> WidgetClass) {
  if (!LayerMap.Contains(LayerTag)) {
    return nullptr;
  }

  return LayerMap[LayerTag]->AddWidget(WidgetClass);
}