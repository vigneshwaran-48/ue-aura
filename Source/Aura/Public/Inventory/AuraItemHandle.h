#pragma once

#include "CoreMinimal.h"
#include "AuraItemHandle.generated.h"

USTRUCT(BlueprintType)
struct AURA_API FAuraItemHandle {
  GENERATED_BODY()

 private:
  UPROPERTY()
  int32 Id = INDEX_NONE;

 public:
  bool IsValid() const { return Id != INDEX_NONE; }

  bool operator==(const FAuraItemHandle& Other) const { return Id == Other.Id; }

  friend uint32 GetTypeHash(const FAuraItemHandle& Handle) {
    return ::GetTypeHash(Handle.Id);
  }

  friend class UAuraInventoryComponent;
};