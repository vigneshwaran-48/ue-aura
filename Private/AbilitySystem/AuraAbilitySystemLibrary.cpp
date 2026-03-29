#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AuraAbilitySystemComponent.h"

UAuraAbilitySystemComponent*
UAuraAbilitySystemLibrary::GetAuraAbilitySystemComponent(AActor* Actor) {
  if (!Actor) {
    return nullptr;
  }

  if (const IAbilitySystemInterface* ASCInterface =
          Cast<IAbilitySystemInterface>(Actor)) {
    return Cast<UAuraAbilitySystemComponent>(
        ASCInterface->GetAbilitySystemComponent());
  }

  return Actor->FindComponentByClass<UAuraAbilitySystemComponent>();
}