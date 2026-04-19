#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "CoreMinimal.h"
#include "Inventory/AuraItemHandle.h"
#include "Inventory/AuraItemInstance.h"
#include "AuraInventoryItemWidget.generated.h"

UCLASS()
class AURA_API UAuraInventoryItemWidget : public UUserWidget {
  GENERATED_BODY()

 public:
  void InitFromItem(const FAuraItemInstance& Item, FAuraItemHandle InHandle,
                    float InCellSize);

 protected:
  virtual FReply NativeOnMouseButtonDown(
      const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

  virtual void NativeOnDragDetected(const FGeometry& InGeometry,
                                    const FPointerEvent& InMouseEvent,
                                    UDragDropOperation*& OutOperation) override;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<USizeBox> RootSizeBox;

 private:
  const FAuraItemInstance* CachedItem = nullptr;
  FAuraItemHandle ItemHandle;
  FIntPoint ItemSize = FIntPoint(1, 1);
  float CellSize = 100.0f;
};