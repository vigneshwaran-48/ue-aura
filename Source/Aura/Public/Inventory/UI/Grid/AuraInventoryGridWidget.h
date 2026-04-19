#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "AuraInventoryGridWidget.generated.h"

class UUniformGridPanel;
class USizeBox;
class UCanvasPanel;

class UAuraInventorySlotWidget;
class UAuraInventoryComponent;
class UAuraInventoryItemWidget;

UCLASS()
class AURA_API UAuraInventoryGridWidget : public UUserWidget {
  GENERATED_BODY()

 public:
  virtual void NativeConstruct() override;

 protected:
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UUniformGridPanel> GridPanel;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<USizeBox> GridSizeBox;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UCanvasPanel> ItemCanvas;

  UPROPERTY(EditDefaultsOnly, Category = "Grid")
  TSubclassOf<UAuraInventorySlotWidget> SlotWidgetClass;

  UPROPERTY(EditDefaultsOnly, Category = "Grid")
  TSubclassOf<UAuraInventoryItemWidget> ItemWidgetClass;

  virtual bool NativeOnDrop(const FGeometry& InGeometry,
                            const FDragDropEvent& InDragDropEvent,
                            UDragDropOperation* InOperation) override;

 private:
  void BuildGrid();
  void PopulateItems();
  UAuraInventoryComponent* GetInventoryComponent() const;
};