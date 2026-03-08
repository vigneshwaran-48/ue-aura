#pragma once

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Interaction/AuraInteractionType.h"
#include "Interaction/AuraInteractionOption.h"
#include "CoreMinimal.h"
#include "AuraGameplayAbility_Interact.generated.h"

class UAuraInteractionComponent;

/**
 * Handles player interaction input.
 * Supports Instant, Hold and Mash interactions.
 */
UCLASS()
class AURA_API UAuraGameplayAbility_Interact : public UGameplayAbility {
  GENERATED_BODY()

 public:
  virtual void ActivateAbility(
      const FGameplayAbilitySpecHandle Handle,
      const FGameplayAbilityActorInfo* ActorInfo,
      const FGameplayAbilityActivationInfo ActivationInfo,
      const FGameplayEventData* TriggerEventData) override;

  virtual void CancelAbility(
      const FGameplayAbilitySpecHandle Handle,
      const FGameplayAbilityActorInfo* ActorInfo,
      const FGameplayAbilityActivationInfo ActivationInfo,
      bool bReplicateCancelAbility) override;

  virtual void InputReleased(
      const FGameplayAbilitySpecHandle Handle,
      const FGameplayAbilityActorInfo* ActorInfo,
      const FGameplayAbilityActivationInfo ActivationInfo) override;

  virtual void InputPressed(
      const FGameplayAbilitySpecHandle Handle,
      const FGameplayAbilityActorInfo* ActorInfo,
      const FGameplayAbilityActivationInfo ActivationInfo);

 protected:
  /* ---------- Interaction Behaviours ---------- */

  void HoldInteract(const FAuraInteractionOption& Option);

  void MashInteract(const FAuraInteractionOption& Option);

  /* ---------- Callbacks ---------- */

  UFUNCTION()
  void OnHoldCompleted();

  UFUNCTION()
  void OnMashPressed(float TimeWaited);

  UFUNCTION()
  void OnMashTimeExpired();

  /* ---------- Helpers ---------- */

  void ExecuteInteraction();

  /* ---------- State ---------- */

  UPROPERTY()
  TObjectPtr<UAuraInteractionComponent> InteractionComponent;

  FAuraInteractionOption ActiveOption;

  int32 MashCount = 0;

  bool bMashActive = false;

  /* ---------- Ability Tasks ---------- */

  UPROPERTY()
  TObjectPtr<UAbilityTask_WaitDelay> HoldDelayTask;

  UPROPERTY()
  TObjectPtr<UAbilityTask_WaitInputRelease> WaitReleaseTask;

  UPROPERTY()
  TObjectPtr<UAbilityTask_WaitInputPress> WaitPressTask;

  UPROPERTY()
  TObjectPtr<UAbilityTask_WaitDelay> MashTimerTask;
};