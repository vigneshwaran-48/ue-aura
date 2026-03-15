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

  InteractableComponent = InteractionComponent->GetInteractableComponent();

  if (!InteractableComponent) {
    UE_LOG(LogTemp, Warning, TEXT("No interactable found!"));
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    return;
  }

  TArray<FAuraInteractionOption> Options =
      InteractableComponent->GetInteractionOptions();

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
  if (!bMashActive) return;

  MashProgress += ActiveOption.MashIncrement;

  UE_LOG(LogTemp, Log, TEXT("Mash pressed! Progress: %f"), MashProgress);

  if (MashProgress >= ActiveOption.MashTargetProgress) {
    bMashActive = false;

    UE_LOG(LogTemp, Log, TEXT("Mash interaction completed!"));

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

  MashProgress = 0.f;
  bMashActive = true;

  MashTickTask = UAbilityTask_WaitDelay::WaitDelay(this, 0.1f);
  MashTickTask->OnFinish.AddDynamic(this,
                                    &UAuraGameplayAbility_Interact::OnMashTick);
  MashTickTask->ReadyForActivation();
}

void UAuraGameplayAbility_Interact::OnMashTick() {
  if (!bMashActive) return;

  MashProgress -= ActiveOption.MashDecayRate * 0.1f;

  MashProgress = FMath::Max(0.f, MashProgress);

  MashTickTask = UAbilityTask_WaitDelay::WaitDelay(this, 0.1f);
  MashTickTask->OnFinish.AddDynamic(this,
                                    &UAuraGameplayAbility_Interact::OnMashTick);
  MashTickTask->ReadyForActivation();
}

void UAuraGameplayAbility_Interact::ExecuteInteraction() {
  if (InteractableComponent) {
    UE_LOG(LogTemp, Log, TEXT("Interacting with interactable!"));
    InteractableComponent->Interact(GetOwningActorFromActorInfo());
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