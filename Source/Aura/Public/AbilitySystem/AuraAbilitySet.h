#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "AuraAbilitySet.generated.h"

class UAuraAbilitySystemComponent;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;

USTRUCT(BlueprintType)
struct FAuraAbilitySet_GameplayAbility {
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<UGameplayAbility> Ability;

  UPROPERTY(EditDefaultsOnly)
  int32 AbilityLevel = 1;

  UPROPERTY(EditDefaultsOnly)
  FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FAuraAbilitySet_GrantedHandles {
  GENERATED_BODY()

 public:
  void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
  void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
  void AddAttributeSet(UAttributeSet* Set);

  void TakeFromAbilitySystem(UAuraAbilitySystemComponent* ASC);

 private:
  UPROPERTY()
  TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

  UPROPERTY()
  TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

  UPROPERTY()
  TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};

UCLASS()
class AURA_API UAuraAbilitySet : public UDataAsset {
  GENERATED_BODY()

 public:
  UPROPERTY(EditDefaultsOnly)
  TArray<FAuraAbilitySet_GameplayAbility> Abilities;

  void GiveToAbilitySystem(
      UAuraAbilitySystemComponent* ASC,
      FAuraAbilitySet_GrantedHandles& OutGrantedHandles) const;
};