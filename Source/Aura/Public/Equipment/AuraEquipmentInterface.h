#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AuraEquipmentInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAuraEquipmentInterface : public UInterface {
  GENERATED_BODY()
};

class AURA_API IAuraEquipmentInterface {
  GENERATED_BODY()

 public:
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Equipment")
  USceneComponent* GetEquipmentAttachComponent(FName SocketName) const;
};
