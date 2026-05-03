#include "Inventory/UI/Grid/AuraInventoryGridWidget.h"

#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "GameFramework/Pawn.h"
#include "Inventory/AuraInventoryComponent.h"
#include "Inventory/Fragments/AuraItemFragment_Size.h"
#include "Inventory/Layouts/AuraGridInventoryLayout.h"
#include "Inventory/UI/Grid/AuraInventoryDragDropOperation.h"
#include "Inventory/UI/Grid/AuraInventoryGhostWidget.h"
#include "Inventory/UI/Grid/AuraInventoryItemWidget.h"
#include "Inventory/UI/Grid/AuraInventorySlotWidget.h"

void UAuraInventoryGridWidget::NativeConstruct() {
  Super::NativeConstruct();

  BuildGrid();
  PopulateItems();
}

UAuraInventoryComponent* UAuraInventoryGridWidget::GetInventoryComponent()
    const {
  APawn* Pawn = GetOwningPlayerPawn();
  if (!Pawn) {
    return nullptr;
  }

  return Pawn->FindComponentByClass<UAuraInventoryComponent>();
}

void UAuraInventoryGridWidget::BuildGrid() {
  if (!GridPanel || !SlotWidgetClass) {
    return;
  }

  UAuraInventoryComponent* Inventory = GetInventoryComponent();
  if (!Inventory) {
    UE_LOG(LogTemp, Warning, TEXT("No InventoryComponent"));
    return;
  }

  UAuraGridInventoryLayout* GridLayout =
      Cast<UAuraGridInventoryLayout>(Inventory->GetLayout());

  if (!GridLayout) {
    UE_LOG(LogTemp, Warning, TEXT("Layout not grid layout"));
    return;
  }

  const int32 Rows = GridLayout->GetRows();
  const int32 Cols = GridLayout->GetColumns();
  const float CellSize = GridLayout->GetCellSize();

  GridPanel->ClearChildren();

  if (GridSizeBox) {
    GridSizeBox->SetWidthOverride(Cols * CellSize);
    GridSizeBox->SetHeightOverride(Rows * CellSize);
  }

  for (int32 Row = 0; Row < Rows; ++Row) {
    for (int32 Col = 0; Col < Cols; ++Col) {
      UAuraInventorySlotWidget* NewSlot =
          CreateWidget<UAuraInventorySlotWidget>(this, SlotWidgetClass);

      if (!NewSlot) {
        continue;
      }

      NewSlot->SetSlotSize(CellSize);

      UUniformGridSlot* GridSlot =
          GridPanel->AddChildToUniformGrid(NewSlot, Row, Col);

      if (GridSlot) {
        GridSlot->SetHorizontalAlignment(HAlign_Fill);
        GridSlot->SetVerticalAlignment(VAlign_Fill);
      }
    }
  }

  UE_LOG(LogTemp, Warning, TEXT("Grid built %dx%d CellSize %.1f"), Rows, Cols,
         CellSize);
}

void UAuraInventoryGridWidget::PopulateItems() {
  if (!ItemCanvas || !ItemWidgetClass) {
    return;
  }

  TArray<UWidget*> Children = ItemCanvas->GetAllChildren();
  for (UWidget* Child : Children) {
    if (Child != Cast<UWidget>(GhostWidget)) {
      Child->RemoveFromParent();
    }
  }

  UAuraInventoryComponent* Inventory = GetInventoryComponent();
  if (!Inventory) {
    return;
  }

  UAuraGridInventoryLayout* Layout =
      Cast<UAuraGridInventoryLayout>(Inventory->GetLayout());

  if (!Layout) {
    return;
  }

  const float CellSize = Layout->GetCellSize();

  TArray<FAuraItemHandle> Handles;
  Layout->GetAllItems(Handles);

  for (const FAuraItemHandle& Handle : Handles) {
    const FAuraItemInstance* Item = Inventory->FindItem(Handle);
    if (!Item) {
      continue;
    }

    FIntPoint Position;
    if (!Layout->GetItemPosition(Handle, Position)) {
      continue;
    }

    const UAuraItemFragment_Size* SizeFragment =
        Item->FindFragment<UAuraItemFragment_Size>();

    const FIntPoint Size = SizeFragment ? SizeFragment->Size : FIntPoint(1, 1);

    UAuraInventoryItemWidget* ItemWidget =
        CreateWidget<UAuraInventoryItemWidget>(this, ItemWidgetClass);

    if (!ItemWidget) {
      continue;
    }

    ItemWidget->InitFromItem(*Item, Handle, Layout->GetCellSize());
    ItemWidget->OwningGrid = this;

    UCanvasPanelSlot* CanvasSlot = ItemCanvas->AddChildToCanvas(ItemWidget);

    if (!CanvasSlot) {
      continue;
    }

    CanvasSlot->SetAutoSize(false);

    CanvasSlot->SetPosition(
        FVector2D(Position.X * CellSize, Position.Y * CellSize));

    CanvasSlot->SetSize(FVector2D(Size.X * CellSize, Size.Y * CellSize));
  }
}

bool UAuraInventoryGridWidget::NativeOnDrop(
    const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation) {
  UAuraInventoryDragDropOperation* DragOp =
      Cast<UAuraInventoryDragDropOperation>(InOperation);
  if (!DragOp) return false;

  UAuraInventoryComponent* Inventory = GetInventoryComponent();
  UAuraGridInventoryLayout* Layout =
      Inventory ? Cast<UAuraGridInventoryLayout>(Inventory->GetLayout())
                : nullptr;

  if (Layout && GhostWidget &&
      GhostWidget->GetVisibility() != ESlateVisibility::Collapsed) {
    UCanvasPanelSlot* GhostSlot = Cast<UCanvasPanelSlot>(GhostWidget->Slot);
    if (GhostSlot) {
      FVector2D GhostPos = GhostSlot->GetPosition();
      const float CellSize = Layout->GetCellSize();

      FIntPoint DropCell(FMath::RoundToInt(GhostPos.X / CellSize),
                         FMath::RoundToInt(GhostPos.Y / CellSize));

      bool bPlaced = Layout->TryAddItemAt(DragOp->ItemHandle, DropCell);

      if (!bPlaced) {
        Layout->TryAddItemAt(DragOp->ItemHandle, DragOp->OriginalPosition);

        if (DragOp->SourceWidget) {
          DragOp->SourceWidget->SetVisibility(ESlateVisibility::Visible);
        }
      } else {
        if (DragOp->SourceWidget) {
          DragOp->SourceWidget->RemoveFromParent();
        }
      }
    }
  }

  if (GhostWidget) {
    GhostWidget->SetVisibility(ESlateVisibility::Collapsed);
  }

  PopulateItems();
  return true;
}

bool UAuraInventoryGridWidget::NativeOnDragOver(
    const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation) {
  Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

  if (UAuraInventoryDragDropOperation* DragOp =
          Cast<UAuraInventoryDragDropOperation>(InOperation)) {
    UpdateGhostPreview(InGeometry, InDragDropEvent, DragOp);
    return true;
  }
  return false;
}

void UAuraInventoryGridWidget::NativeOnDragLeave(
    const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) {
  Super::NativeOnDragLeave(InDragDropEvent, InOperation);

  if (GhostWidget) {
    GhostWidget->SetVisibility(ESlateVisibility::Collapsed);
  }
}

void UAuraInventoryGridWidget::UpdateGhostPreview(
    const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UAuraInventoryDragDropOperation* DragOp) {
  UAuraInventoryComponent* Inventory = GetInventoryComponent();
  if (!Inventory || !ItemCanvas || !GhostWidgetClass) return;

  UAuraGridInventoryLayout* Layout =
      Cast<UAuraGridInventoryLayout>(Inventory->GetLayout());
  if (!Layout) return;

  if (!GhostWidget) {
    GhostWidget =
        CreateWidget<UAuraInventoryGhostWidget>(this, GhostWidgetClass);
    UCanvasPanelSlot* GhostSlot = ItemCanvas->AddChildToCanvas(GhostWidget);
    GhostSlot->SetZOrder(-1);  // Keep it behind the actual items
  }

  const float CellSize = Layout->GetCellSize();
  FVector2D LocalPos =
      InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());

  FVector2D ItemTopLeft = LocalPos - DragOp->DragOffset;
  FIntPoint GhostCell(FMath::RoundToInt(ItemTopLeft.X / CellSize),
                      FMath::RoundToInt(ItemTopLeft.Y / CellSize));

  GhostWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
  UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(GhostWidget->Slot);
  if (CanvasSlot) {
    CanvasSlot->SetPosition(
        FVector2D(GhostCell.X * CellSize, GhostCell.Y * CellSize));
    CanvasSlot->SetSize(FVector2D(DragOp->ItemSize.X * CellSize,
                                  DragOp->ItemSize.Y * CellSize));
  }

  bool bCanPlace = Layout->CanPlaceItemAt(GhostCell, DragOp->ItemSize);
  EGhostWidgetState TargetState =
      bCanPlace ? EGhostWidgetState::Valid : EGhostWidgetState::Invalid;
  GhostWidget->SetGhostState(TargetState);
}