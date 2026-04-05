#include "AbilitySystem/AuraAbilitySet.h"

#include "Abilities/GameplayAbility.h"
#include "AuraAbilitySystemComponent.h"

void UAuraAbilitySet::GiveToAbilitySystem(
    UAuraAbilitySystemComponent* ASC,
    FAuraAbilitySet_GrantedHandles& OutGrantedHandles) const {
  if (!ASC) return;

  for (const FAuraAbilitySet_GameplayAbility& Ability : Abilities) {
    if (!Ability.Ability) continue;

    FGameplayAbilitySpec Spec(Ability.Ability, Ability.AbilityLevel);

    if (Ability.InputTag.IsValid()) {
      Spec.DynamicAbilityTags.AddTag(Ability.InputTag);
    }

    FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(Spec);

    OutGrantedHandles.AddAbilitySpecHandle(Handle);
  }
}

void FAuraAbilitySet_GrantedHandles::AddAbilitySpecHandle(
    const FGameplayAbilitySpecHandle& Handle) {
  if (Handle.IsValid()) {
    AbilitySpecHandles.Add(Handle);
  }
}

void FAuraAbilitySet_GrantedHandles::AddGameplayEffectHandle(
    const FActiveGameplayEffectHandle& Handle) {
  if (Handle.IsValid()) {
    GameplayEffectHandles.Add(Handle);
  }
}

void FAuraAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set) {
  GrantedAttributeSets.Add(Set);
}

void FAuraAbilitySet_GrantedHandles::TakeFromAbilitySystem(
    UAuraAbilitySystemComponent* ASC) {
  if (!ASC) return;

  for (FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles) {
    if (Handle.IsValid()) {
      ASC->ClearAbility(Handle);
    }
  }

  for (FActiveGameplayEffectHandle& Handle : GameplayEffectHandles) {
    if (Handle.IsValid()) {
      ASC->RemoveActiveGameplayEffect(Handle);
    }
  }

  AbilitySpecHandles.Reset();
  GameplayEffectHandles.Reset();
  GrantedAttributeSets.Reset();
}