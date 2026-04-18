#pragma once

#include "Inventory/AuraItemHandle.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraInventoryLibrary.generated.h"

UCLASS()
class AURA_API UAuraInventoryBPLibrary : public UBlueprintFunctionLibrary {
  GENERATED_BODY()

 public:
  UFUNCTION(BlueprintPure, Category = "Inventory")
  static bool IsValidItemHandle(FAuraItemHandle Handle) {
    return Handle.IsValid();
  }

  UFUNCTION(BlueprintPure, Category = "Inventory")
  static bool Equal_ItemHandle(FAuraItemHandle A, FAuraItemHandle B) {
    return A == B;
  }
};