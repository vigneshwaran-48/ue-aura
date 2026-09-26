#include "Inventory/UI/Grid/AuraInventoryInteractController.h"

#include "Inventory/AuraInventoryComponent.h"
#include "Inventory/Layouts/AuraGridInventoryLayout.h"
#include "Inventory/UI/Grid/AuraInventoryGridWidget.h"

void UAuraInventoryInteractController::Initialize(
	UAuraInventoryGridWidget* InGridWidget)
{
	GridWidget = InGridWidget;

	Inventory = nullptr;
	Layout = nullptr;

	if (!GridWidget)
		return;

	Inventory = GridWidget->GetInventoryComponent();

	if (Inventory) {
		Layout = Cast<UAuraGridInventoryLayout>(
			Inventory->GetLayout());
	}
}

void UAuraInventoryInteractController::Reset()
{
	CancelItemMove();
	ClearSelection();

	GridWidget = nullptr;
	Inventory = nullptr;
	Layout = nullptr;
}

void UAuraInventoryInteractController::InitializeSelection()
{
	if (!Layout) {
		ClearSelection();
		return;
	}

	TArray<FAuraItemHandle> Handles;
	Layout->GetAllItems(Handles);

	if (Handles.Num() > 0) {
		FIntPoint Position;

		if (Layout->GetItemPosition(Handles[0], Position)) {
			SetSelection(Position);
			return;
		}
	}

	SetSelection(FIntPoint::ZeroValue);
}

void UAuraInventoryInteractController::ClearSelection()
{
	bHasSelection = false;
	SelectedCell = FIntPoint::ZeroValue;

	if (GridWidget)
		GridWidget->UpdateControllerSelectionVisual();
}

void UAuraInventoryInteractController::MoveSelection(
	FIntPoint Direction)
{
	if (!Layout || Direction == FIntPoint::ZeroValue)
		return;

	if (!bHasSelection) {
		InitializeSelection();
		return;
	}

	const FIntPoint NextCell =
		FindNextSelectionCell(
			SelectedCell,
			Direction);

	if (!IsValidCell(NextCell))
		return;

	if (MoveState.bIsActive) {
		UpdateItemMove(NextCell);
	}
	else {
		SetSelection(NextCell);
	}
}

bool UAuraInventoryInteractController::BeginItemMove()
{
	FAuraItemHandle Handle;

	if (!GetSelectedItem(Handle))
		return false;

	return BeginItemMove(Handle);
}

bool UAuraInventoryInteractController::BeginItemMove(
	const FAuraItemHandle& Handle)
{
	if (!Layout || MoveState.bIsActive)
		return false;

	FIntPoint OriginalPosition;

	if (!Layout->GetItemPosition(Handle, OriginalPosition))
		return false;

	const FIntPoint ItemSize = Layout->GetItemSize(Handle);

	Layout->RemoveItem(Handle);

	MoveState.ItemHandle = Handle;
	MoveState.OriginalPosition = OriginalPosition;
	MoveState.CurrentPosition = OriginalPosition;
	MoveState.ItemSize = ItemSize;
	MoveState.bIsActive = true;

	SetSelection(OriginalPosition);

	if (GridWidget)
		GridWidget->PopulateItems();

	return true;
}

void UAuraInventoryInteractController::UpdateItemMove(
	FIntPoint Position)
{
	if (!MoveState.bIsActive)
		return;

	MoveState.CurrentPosition = Position;

	SelectedCell = Position;
	bHasSelection = true;

	if (GridWidget)
		GridWidget->UpdateControllerMoveVisual();
}

bool UAuraInventoryInteractController::CommitItemMove()
{
	if (!Layout || !MoveState.bIsActive)
		return false;

	if (!CanPlaceAt(MoveState.CurrentPosition))
		return false;

	if (!Layout->TryAddItemAt(
		MoveState.ItemHandle,
		MoveState.CurrentPosition)) {
		return false;
	}

	MoveState = {};

	if (GridWidget) {
		GridWidget->ClearControllerMoveVisual();
		GridWidget->PopulateItems();
		GridWidget->UpdateControllerSelectionVisual();
	}

	return true;
}

bool UAuraInventoryInteractController::CancelItemMove()
{
	if (!MoveState.bIsActive)
		return false;

	if (!Layout)
		return false;

	const bool bRestored =
		Layout->TryAddItemAt(
			MoveState.ItemHandle,
			MoveState.OriginalPosition);

	if (!bRestored)
		return false;

	MoveState = {};

	if (GridWidget) {
		GridWidget->ClearControllerMoveVisual();
		GridWidget->PopulateItems();
		GridWidget->UpdateControllerSelectionVisual();
	}

	return true;
}

bool UAuraInventoryInteractController::CanPlaceAt(
	FIntPoint Position) const
{
	if (!Layout || !MoveState.bIsActive)
		return false;

	return Layout->CanPlaceItemAt(
		Position,
		MoveState.ItemSize);
}

FIntPoint UAuraInventoryInteractController::FindNextSelectionCell(
	FIntPoint CurrentCell,
	FIntPoint Direction) const
{
	if (!Layout) {
		return CurrentCell;
	}
	
	FAuraItemHandle Handle;

	if (Layout->GetItemAtCell(CurrentCell, Handle)) {
		FIntPoint Position;

		if (!Layout->GetItemPosition(Handle, Position)) {
			return CurrentCell + Direction;
		}

		const FIntPoint Size =
			Layout->GetItemSize(Handle);

		if (Direction.X > 0) {
			return FIntPoint(
				Position.X + Size.X,
				CurrentCell.Y);
		}

		if (Direction.X < 0) {
			return FIntPoint(
				Position.X - 1,
				CurrentCell.Y);
		}

		if (Direction.Y > 0) {
			return FIntPoint(
				CurrentCell.X,
				Position.Y + Size.Y);
		}

		if (Direction.Y < 0) {
			return FIntPoint(
				CurrentCell.X,
				Position.Y - 1);
		}
	}

	return CurrentCell + Direction;
}

bool UAuraInventoryInteractController::IsValidCell(
	FIntPoint Cell) const
{
	if (!Layout)
		return false;

	return Cell.X >= 0 &&
		Cell.Y >= 0 &&
		Cell.X < Layout->GetColumns() &&
		Cell.Y < Layout->GetRows();
}

bool UAuraInventoryInteractController::GetSelectedItem(
	FAuraItemHandle& OutHandle) const
{
	if (!Layout || !bHasSelection)
		return false;

	return Layout->GetItemAtCell(
		SelectedCell,
		OutHandle);
}

void UAuraInventoryInteractController::SetSelection(
	FIntPoint Cell)
{
	if (!IsValidCell(Cell))
		return;

	SelectedCell = Cell;
	bHasSelection = true;

	if (GridWidget && !MoveState.bIsActive)
		GridWidget->UpdateControllerSelectionVisual();
}

void UAuraInventoryInteractController::HandleConfirm()
{
	if (MoveState.bIsActive) {
		if (!CanPlaceAt(MoveState.CurrentPosition)) {
			CancelItemMove();
		}
		else {
			PlaceHeldItem();
		}

		return;
	}

	PickUpSelectedItem();
}

void UAuraInventoryInteractController::HandleCancel()
{
	if (MoveState.bIsActive) {
		CancelItemMove();
		return;
	}

	ClearSelection();
}

bool UAuraInventoryInteractController::PickUpSelectedItem()
{
	if (!bHasSelection || MoveState.bIsActive)
		return false;

	FAuraItemHandle Handle;

	if (!GetSelectedItem(Handle))
		return false;

	return BeginItemMove(Handle);
}

bool UAuraInventoryInteractController::PlaceHeldItem()
{
	return CommitItemMove();
}