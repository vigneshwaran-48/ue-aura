#include "UI/AuraUITags.h"

#include "GameplayTagsManager.h"

FGameplayTag FAuraUITags::UI_Layer_Game;
FGameplayTag FAuraUITags::UI_Layer_HUD;
FGameplayTag FAuraUITags::UI_Layer_Menu;
FGameplayTag FAuraUITags::UI_Layer_Modal;

void FAuraUITags::InitializeTags() {
  UI_Layer_Game =
      UGameplayTagsManager::Get().AddNativeGameplayTag(TEXT("UI.Layer.Game"));
  UI_Layer_HUD =
      UGameplayTagsManager::Get().AddNativeGameplayTag(TEXT("UI.Layer.HUD"));
  UI_Layer_Menu =
      UGameplayTagsManager::Get().AddNativeGameplayTag(TEXT("UI.Layer.Menu"));
  UI_Layer_Modal =
      UGameplayTagsManager::Get().AddNativeGameplayTag(TEXT("UI.Layer.Modal"));
}