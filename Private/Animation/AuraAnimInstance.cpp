#include "Animation/AuraAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffectTypes.h"

UAuraAnimInstance::UAuraAnimInstance(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {}

void UAuraAnimInstance::InitializeWithAbilitySystem(
    UAbilitySystemComponent* ASC) {
  check(ASC);

  GameplayTagPropertyMap.Initialize(this, ASC);

  UE_LOG(LogTemp, Log,
         TEXT("UAuraAnimInstance::InitializeWithAbilitySystem: Initializing ASC "
              "on AnimInstance %s with ASC from Actor %s"),
      *GetName(), *ASC->GetOwner()->GetName());
}

void UAuraAnimInstance::NativeInitializeAnimation() {
  Super::NativeInitializeAnimation();

  if (AActor* OwningActor = TryGetPawnOwner()) {
    if (UAbilitySystemComponent* ASC =
            UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(
                OwningActor)) {
      InitializeWithAbilitySystem(ASC);
    } else {
      UE_LOG(LogTemp, Warning,
             TEXT("UAuraAnimInstance::NativeInitializeAnimation: Failed to "
                  "find ASC on Actor %s"),
             *OwningActor->GetName());
    }
  }
}
