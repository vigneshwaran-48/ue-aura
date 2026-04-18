#pragma once

#include "AuraItemDefinition.h"
#include "CoreMinimal.h"
#include "AuraItemInstance.generated.h"

USTRUCT(BlueprintType)
struct FAuraItemInstance {
  GENERATED_BODY()

  UPROPERTY()
  TObjectPtr<const UAuraItemDefinition> Definition = nullptr;

  template <typename T>
  const T* FindFragment() const {
    return Definition ? Definition->FindFragment<T>() : nullptr;
  }
};