#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "UI/AuraActivatableWidget.h"
#include "Inventory/AuraItemHandle.h"
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
class AURA_API UAuraInventoryGridWidget : public UAuraActivatableWidget {
  GENERATED_BODY()

 public:
  virtual void NativeConstruct() override;

  void PopulateItems();

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

  virtual void NativeOnActivated() override;

  virtual void NativeOnDeactivated() override;

  virtual FReply NativeOnKeyDown(
      const FGeometry& InGeometry,
      const FKeyEvent& InKeyEvent) override;

 private:
  UPROPERTY()
  UAuraInventoryGhostWidget* GhostWidget;

  UPROPERTY(meta = (BindWidgetOptional))
  UWidget* SelectionWidget;

  UPROPERTY()
  FIntPoint SelectedCell = FIntPoint::ZeroValue;

  UPROPERTY()
  bool bHasControllerSelection = false;

  UPROPERTY()
  bool bControllerHoldingItem = false;

  UPROPERTY()
  FAuraItemHandle HeldItemHandle;

  UPROPERTY()
  FIntPoint HeldItemOriginalPosition = FIntPoint::ZeroValue;

  UPROPERTY()
  FIntPoint HeldItemSize = FIntPoint(1, 1);

  UPROPERTY()
  bool bUsingController = false;

  void BuildGrid();

  UAuraInventoryComponent* GetInventoryComponent() const;

  void UpdateGhostPreview(const FGeometry& InGeometry,
                          const FDragDropEvent& InDragDropEvent,
                          class UAuraInventoryDragDropOperation* DragOp);

  void InitializeControllerSelection();

  void MoveSelection(FIntPoint Direction);

  void HandleControllerConfirm();

  void HandleControllerCancel();

  bool PickUpSelectedItem();

  bool PlaceHeldItem();

  void SetControllerSelection(FIntPoint Cell);

  void UpdateControllerSelectionVisual();

  void SetGhostCell(
      FIntPoint Cell,
      bool bShowGhost = true);

  bool IsValidCell(FIntPoint Cell) const;
};