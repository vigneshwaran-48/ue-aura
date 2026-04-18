#pragma once

#include "CoreMinimal.h"
#include "Inventory/AuraItemHandle.h"
#include "UObject/Object.h"
#include "AuraInventoryLayout.generated.h"

class UAuraInventoryComponent;

UCLASS(Abstract, Blueprintable, EditInlineNew)
class AURA_API UAuraInventoryLayout : public UObject {
  GENERATED_BODY()

 public:
  virtual void Initialize(UAuraInventoryComponent* InInventory) {
    Inventory = InInventory;
  }

  virtual bool TryAddItem(const FAuraItemHandle& Handle)
      PURE_VIRTUAL(UAuraInventoryLayout::TryAddItem, return false;);

  virtual void RemoveItem(const FAuraItemHandle& Handle)
      PURE_VIRTUAL(UAuraInventoryLayout::RemoveItem, );

 protected:
  UPROPERTY()
  TObjectPtr<UAuraInventoryComponent> Inventory;
};