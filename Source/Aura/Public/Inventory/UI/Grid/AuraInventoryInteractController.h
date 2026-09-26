#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Inventory/AuraItemHandle.h"
#include "AuraInventoryInteractController.generated.h"

class UAuraInventoryGridWidget;
class UAuraInventoryComponent;
class UAuraGridInventoryLayout;

USTRUCT()
struct FAuraInventoryMoveState
{
    GENERATED_BODY()

    FAuraItemHandle ItemHandle;
    FIntPoint OriginalPosition = FIntPoint::ZeroValue;
    FIntPoint CurrentPosition = FIntPoint::ZeroValue;
    FIntPoint ItemSize = FIntPoint(1, 1);

    bool bIsActive = false;
};

UCLASS()
class AURA_API UAuraInventoryInteractController : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UAuraInventoryGridWidget* InGridWidget);

    void Reset();

    // Selection
    void InitializeSelection();
    void ClearSelection();

    bool HasSelection() const { return bHasSelection; }
    FIntPoint GetSelectedCell() const { return SelectedCell; }

    void MoveSelection(FIntPoint Direction);

    // Item movement
    bool BeginItemMove();
    bool BeginItemMove(const FAuraItemHandle& Handle);

    void UpdateItemMove(FIntPoint Position);

    bool CommitItemMove();
    bool CancelItemMove();

    void HandleConfirm();
    void HandleCancel();

    bool PickUpSelectedItem();
    bool PlaceHeldItem();

    void ClearMoveState();

    bool IsMovingItem() const { return MoveState.bIsActive; }

    const FAuraInventoryMoveState& GetMoveState() const
    {
        return MoveState;
    }

    bool CanPlaceAt(FIntPoint Position) const;

private:
    UPROPERTY()
    TObjectPtr<UAuraInventoryGridWidget> GridWidget;

    UPROPERTY()
    TObjectPtr<UAuraInventoryComponent> Inventory;

    UPROPERTY()
    TObjectPtr<UAuraGridInventoryLayout> Layout;

    FIntPoint SelectedCell = FIntPoint::ZeroValue;
    bool bHasSelection = false;

    FAuraInventoryMoveState MoveState;

    FIntPoint FindNextSelectionCell(
        FIntPoint CurrentCell,
        FIntPoint Direction) const;

    bool IsValidCell(FIntPoint Cell) const;

    bool GetSelectedItem(FAuraItemHandle& OutHandle) const;

    void SetSelection(FIntPoint Cell);

};