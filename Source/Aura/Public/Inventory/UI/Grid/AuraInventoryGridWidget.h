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
class UAuraInventoryInteractController;

UCLASS()
class AURA_API UAuraInventoryGridWidget : public UAuraActivatableWidget {
  GENERATED_BODY()

 public:
  virtual void NativeConstruct() override;

  void PopulateItems();

  UAuraInventoryComponent* GetInventoryComponent() const;

  void UpdateControllerSelectionVisual();

  void UpdateControllerMoveVisual();

  void ClearControllerMoveVisual();

  bool HandleDropOutsideGrid();

  void BeginItemMoveVisual(const FAuraItemHandle& Handle);

  void UpdateItemMoveVisual();

  void ClearItemMoveVisual();

  UAuraInventoryInteractController* GetInteractionController() const
  {
      return InteractionController;
  }

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

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
  FDataTableRowHandle MoveInputActionData;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
  FDataTableRowHandle CancelInputActionData;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
  FDataTableRowHandle MoveUpInputActionData;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
  FDataTableRowHandle MoveDownInputActionData;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
  FDataTableRowHandle MoveLeftInputActionData;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
  FDataTableRowHandle MoveRightInputActionData;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
  FDataTableRowHandle CloseInventoryInputActionRowHandle;

  virtual bool NativeOnDrop(const FGeometry& InGeometry,
                            const FDragDropEvent& InDragDropEvent,
                            UDragDropOperation* InOperation) override;

  virtual bool NativeOnDragOver(const FGeometry& InGeometry,
                                const FDragDropEvent& InDragDropEvent,
                                UDragDropOperation* InOperation) override;

  virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent,
                                 UDragDropOperation* InOperation) override;

  virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent,
      UDragDropOperation* InOperation) override;

  virtual void NativeOnActivated() override;

  virtual void NativeOnDeactivated() override;

 private:
  UPROPERTY()
  UAuraInventoryGhostWidget* GhostWidget;

  UPROPERTY()
  bool bUsingController = false;

  UPROPERTY()
  TMap<FIntPoint, TObjectPtr<UAuraInventorySlotWidget>> GridSlots;

  UPROPERTY()
  TMap<FAuraItemHandle, TObjectPtr<UAuraInventoryItemWidget>> ItemWidgets;

  UPROPERTY()
  TObjectPtr<UAuraInventoryInteractController> InteractionController;

  UPROPERTY()
  TObjectPtr<UAuraInventoryItemWidget> MovingItemWidget;

  void BuildGrid();

  void SetGhostCell(
      FIntPoint Cell,
      bool bShowGhost = true);

  void RegisterAction(const FDataTableRowHandle& ActionHandle, const FSimpleDelegate& Delegate);

  void MoveSelection(const FIntPoint& Direction);

  void OnMoveSelected();

  void OnCancelPressed();

  void OnCloseRequested();
};