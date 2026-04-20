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
class UAuraInventoryDragDropOperation;
class UAuraInventoryGhostWidget;

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

  UPROPERTY(EditAnywhere, Category = "Grid")
  TSubclassOf<UAuraInventoryGhostWidget> GhostWidgetClass;

  virtual bool NativeOnDrop(const FGeometry& InGeometry,
                            const FDragDropEvent& InDragDropEvent,
                            UDragDropOperation* InOperation) override;

  virtual bool NativeOnDragOver(const FGeometry& InGeometry,
                                const FDragDropEvent& InDragDropEvent,
                                UDragDropOperation* InOperation) override;

  virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent,
                                 UDragDropOperation* InOperation) override;

 private:
  UPROPERTY()
  UAuraInventoryGhostWidget* GhostWidget;

  void BuildGrid();

  void PopulateItems();

  UAuraInventoryComponent* GetInventoryComponent() const;

  void UpdateGhostPreview(const FGeometry& InGeometry,
                          const FDragDropEvent& InDragDropEvent,
                          class UAuraInventoryDragDropOperation* DragOp);
};