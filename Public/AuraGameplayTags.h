#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(
    TAG_Aura_Message_Interaction_TargetChanged);

///////////////////////////////////////////////////////////////
// UI Tags
///////////////////////////////////////////////////////////////

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Action_MainMenu);

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Layer_Game);

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Layer_GameMenu);

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UI_Layer_Menu);

///////////////////////////////////////////////////////////////
// Slot Tags
///////////////////////////////////////////////////////////////
AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AURA_SLOTS_LEGS);

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AURA_SLOTS_PELVIS);

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AURA_SLOTS_SPINE);

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AURA_SLOTS_HEAD);

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AURA_SLOTS_ARM_L);

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AURA_SLOTS_ARM_R);

///////////////////////////////////////////////////////////////
// Movement State Tags
///////////////////////////////////////////////////////////////

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AURA_STATE_MOVEMENT_IDLE);

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AURA_STATE_MOVEMENT_WALK);

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AURA_STATE_MOVEMENT_RUN);

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AURA_STATE_MOVEMENT_SPRINT);

///////////////////////////////////////////////////////////////
// Stance Tags
///////////////////////////////////////////////////////////////

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AURA_STATE_STANCE_STAND);

AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AURA_STATE_STANCE_CROUCH);


AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AURA_STATE_ADS);
