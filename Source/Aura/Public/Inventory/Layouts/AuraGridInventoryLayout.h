#pragma once

#include "AuraInventoryLayout.h"
#include "CoreMinimal.h"
#include "AuraGridInventoryLayout.generated.h"

UCLASS(Blueprintable, EditInlineNew)
class AURA_API UAuraGridInventoryLayout : public UAuraInventoryLayout {
  GENERATED_BODY()

 public:
  UPROPERTY(EditAnywhere, Category = "Grid")
  int32 Rows = 5;

  UPROPERTY(EditAnywhere, Category = "Grid")
  int32 Columns = 8;

  UPROPERTY(EditAnywhere, Category = "Grid")
  float CellSize = 64.f;

  virtual bool TryAddItem(const FAuraItemHandle& Handle) override;

  virtual void RemoveItem(const FAuraItemHandle& Handle) override;

  bool CanPlaceItemAt(FIntPoint Position, FIntPoint Size) const;

  bool IsCellOccupied(FIntPoint Cell) const;

  bool GetItemPosition(const FAuraItemHandle& Handle, FIntPoint& OutPos) const;

  bool TryAddItemAt(const FAuraItemHandle& Handle, FIntPoint Position);

  UFUNCTION(BlueprintCallable)
  int32 GetRows() const { return Rows; }

  UFUNCTION(BlueprintCallable)
  int32 GetColumns() const { return Columns; }

  UFUNCTION(BlueprintCallable)
  float GetCellSize() const { return CellSize; }

  UFUNCTION(BlueprintCallable)
  void GetAllItems(TArray<FAuraItemHandle>& OutHandles) const;

 private:
  TMap<FAuraItemHandle, FIntPoint> ItemPositions;
  TSet<FIntPoint> OccupiedCells;

  FIntPoint GetItemSize(const FAuraItemHandle& Handle) const;
};