#include "Interaction/Abilities/AuraGameplayAbility_Interact.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "Interaction/AuraInteractableComponent.h"
#include "Interaction/AuraInteractionComponent.h"
#include "Interaction/AuraInteractionOption.h"

void UAuraGameplayAbility_Interact::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData) {
  if (!ActorInfo || !ActorInfo->AvatarActor.IsValid()) {
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    return;
  }

  AActor* AvatarActor = ActorInfo->AvatarActor.Get();

  InteractionComponent =
      AvatarActor->FindComponentByClass<UAuraInteractionComponent>();

  if (!InteractionComponent) {
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    return;
  }

  InteractionComponent->UpdateInteractionTarget();

  if (!InteractionComponent->GetInteractableComponent()) {
    UE_LOG(LogTemp, Warning, TEXT("No interactable found!"));
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    return;
  }

  TArray<FAuraInteractionOption> Options =
      InteractionComponent->GetInteractableComponent()->GetInteractionOptions();

  if (Options.Num() == 0) {
    UE_LOG(LogTemp, Warning, TEXT("No interact options found!"));
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    return;
  }

  const FAuraInteractionOption& Option = Options[0];
  const EAuraInteractionType InteractionType = Option.InteractionType;

  switch (InteractionType) {
    case EAuraInteractionType::Instant:
      ExecuteInteraction();
      EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
      break;
    case EAuraInteractionType::Hold:
      HoldInteract(Option);
      break;
    case EAuraInteractionType::Mash:
      MashInteract(Option);
      break;
    default:
      break;
  }
}

void UAuraGameplayAbility_Interact::HoldInteract(
    const FAuraInteractionOption& Option) {
  ActiveOption = Option;

  const float HoldDuration = Option.HoldDuration;

  HoldDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, HoldDuration);
  HoldDelayTask->OnFinish.AddDynamic(
      this, &UAuraGameplayAbility_Interact::OnHoldCompleted);
  HoldDelayTask->ReadyForActivation();
}

void UAuraGameplayAbility_Interact::InputReleased(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo) {
  Super::InputReleased(Handle, ActorInfo, ActivationInfo);
  if (ActiveOption.InteractionType == EAuraInteractionType::Hold) {
    UE_LOG(LogTemp, Log, TEXT("Input released"));
    CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
                  true);
  }
}

void UAuraGameplayAbility_Interact::InputPressed(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo) {
  UE_LOG(LogTemp, Log, TEXT("Input pressed"));
  if (!bMashActive) return;

  MashCount++;

  UE_LOG(LogTemp, Log, TEXT("Mash pressed! Count: %d"), MashCount);

  if (MashCount >= ActiveOption.MashTarget) {
    UE_LOG(LogTemp, Log, TEXT("Mash interaction succeeded!"));
    bMashActive = false;
    MashCount = 0;

    if (MashTimerTask) {
      MashTimerTask->EndTask();
      MashTimerTask = nullptr;
    }

    ExecuteInteraction();

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
  }
}

void UAuraGameplayAbility_Interact::OnHoldCompleted() {
  ExecuteInteraction();

  EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true,
             false);
}

void UAuraGameplayAbility_Interact::MashInteract(
    const FAuraInteractionOption& Option) {
  ActiveOption = Option;

  MashCount = 0;
  bMashActive = true;

  const float TimeLimit = Option.MashTimeLimit;

  MashTimerTask = UAbilityTask_WaitDelay::WaitDelay(this, TimeLimit);
  MashTimerTask->OnFinish.AddDynamic(
      this, &UAuraGameplayAbility_Interact::OnMashTimeExpired);

  MashTimerTask->ReadyForActivation();
}

void UAuraGameplayAbility_Interact::OnMashTimeExpired() {
  if (!bMashActive) return;

  bMashActive = false;

  UE_LOG(LogTemp, Warning, TEXT("Mash interaction failed (time expired)"));

  CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
                true);
}

void UAuraGameplayAbility_Interact::OnMashPressed(float TimeWaited) {
  MashCount++;

  UE_LOG(LogTemp, Log, TEXT("Mash pressed! Count: %d"), MashCount);

  if (MashCount >= ActiveOption.MashTarget) {
    ExecuteInteraction();

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true,
               false);
    return;
  }

  WaitPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this, false);
  WaitPressTask->OnPress.AddDynamic(
      this, &UAuraGameplayAbility_Interact::OnMashPressed);
  WaitPressTask->ReadyForActivation();
}

void UAuraGameplayAbility_Interact::ExecuteInteraction() {
  if (!InteractionComponent) {
    UE_LOG(LogTemp, Warning, TEXT("No interaction component found!"));
  }
  if (!InteractionComponent->GetInteractableComponent()) {
    UE_LOG(LogTemp, Warning, TEXT("No interactable component found in interaction component!"));
  }
  if (InteractionComponent &&
      InteractionComponent->GetInteractableComponent()) {

    UE_LOG(LogTemp, Log, TEXT("Interacting with interactable!"));
    InteractionComponent->GetInteractableComponent()->Interact(
        GetOwningActorFromActorInfo());
  }
}

void UAuraGameplayAbility_Interact::CancelAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateCancelAbility) {
  if (HoldDelayTask) {
    HoldDelayTask->EndTask();
  }

  if (WaitReleaseTask) {
    WaitReleaseTask->EndTask();
  }

  if (WaitPressTask) {
    WaitPressTask->EndTask();
  }

  if (MashTimerTask) {
    MashTimerTask->EndTask();
    MashTimerTask = nullptr;
  }

  Super::CancelAbility(Handle, ActorInfo, ActivationInfo,
                       bReplicateCancelAbility);
}