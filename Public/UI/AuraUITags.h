#pragma once

#include "GameplayTagContainer.h"

struct FAuraUITags {
  static FGameplayTag UI_Layer_Game;
  static FGameplayTag UI_Layer_HUD;
  static FGameplayTag UI_Layer_Menu;
  static FGameplayTag UI_Layer_Modal;

  static void InitializeTags();
};