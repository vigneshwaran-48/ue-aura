#pragma once

#include "AuraInventoryLayout.h"
#include "CoreMinimal.h"
#include "AuraGridInventoryLayout.generated.h"

UCLASS(Blueprintable, EditInlineNew)
class AURA_API UAuraGridInventoryLayout : public UAuraInventoryLayout {
  GENERATED_BODY()

 public:
  UPROPERTY(EditAnywhere)
  int32 Rows = 5;

  UPROPERTY(EditAnywhere)
  int32 Columns = 8;

  virtual bool TryAddItem(const FAuraItemHandle& Handle) override;

  virtual void RemoveItem(const FAuraItemHandle& Handle) override;

  bool CanPlaceItemAt(FIntPoint Position, FIntPoint Size) const;

  bool IsCellOccupied(FIntPoint Cell) const;

  bool GetItemPosition(const FAuraItemHandle& Handle, FIntPoint& OutPos) const;

 private:
  TMap<FAuraItemHandle, FIntPoint> ItemPositions;
  TSet<FIntPoint> OccupiedCells;

  FIntPoint GetItemSize(const FAuraItemHandle& Handle) const;
};