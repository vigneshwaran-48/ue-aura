#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AuraPawnDataProvider.generated.h"

class UAuraPawnData;

UINTERFACE(MinimalAPI)
class UAuraPawnDataProvider : public UInterface {
  GENERATED_BODY()
};

class AURA_API IAuraPawnDataProvider {
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  const UAuraPawnData* GetPawnData() const;
};
