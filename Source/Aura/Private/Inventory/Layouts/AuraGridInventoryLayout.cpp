#include "Inventory/Layouts/AuraGridInventoryLayout.h"

#include "AuraGameplayTags.h"
#include "Inventory/AuraInventoryComponent.h"
#include "Inventory/AuraItemInstance.h"
#include "Inventory/AuraItemDefinition.h"
#include "Inventory/Fragments/AuraItemFragment_LayoutBehavior.h"
#include "Inventory/Fragments/AuraItemFragment_Size.h"

FIntPoint
UAuraGridInventoryLayout::GetItemSize(const FAuraItemHandle& Handle) const {
	const FAuraItemInstance* Item = Inventory->FindItem(Handle);
	if (!Item)
		return FIntPoint(1, 1);

	const UAuraItemFragment_Size* Frag =
		Item->FindFragment<UAuraItemFragment_Size>();
	return Frag ? Frag->Size : FIntPoint(1, 1);
}

bool UAuraGridInventoryLayout::TryAddItem(const FAuraItemHandle& Handle) {
	if (!IsSpatialItem(Handle)) {
		return true;
	}

	const FIntPoint Size = GetItemSize(Handle);

	for (int32 Y = 0; Y < Rows; ++Y) {
		for (int32 X = 0; X < Columns; ++X) {
			const FIntPoint Position(X, Y);

			if (!CanPlaceItemAt(Position, Size)) {
				continue;
			}

			ItemPositions.Add(Handle, Position);
			AddOccupiedCells(Position, Size, OccupiedCells);
			return true;
		}
	}

	return false;
}

bool UAuraGridInventoryLayout::CanPlaceItemAt(
	FIntPoint Position,
	FIntPoint Size) const {
	return CanPlaceItemAt(Position, Size, OccupiedCells);
}

bool UAuraGridInventoryLayout::CanPlaceItemAt(
	FIntPoint Position,
	FIntPoint Size,
	const TSet<FIntPoint>& InOccupiedCells) const {
	if (Position.X < 0 || Position.Y < 0) {
		return false;
	}

	if (Position.X + Size.X > Columns ||
		Position.Y + Size.Y > Rows) {
		return false;
	}

	for (int32 Y = 0; Y < Size.Y; ++Y) {
		for (int32 X = 0; X < Size.X; ++X) {
			if (InOccupiedCells.Contains(Position + FIntPoint(X, Y))) {
				return false;
			}
		}
	}

	return true;
}

void UAuraGridInventoryLayout::RemoveItem(const FAuraItemHandle& Handle) {
	if (!IsSpatialItem(Handle)) {
		return;
	}

	const FIntPoint* Position = ItemPositions.Find(Handle);
	if (!Position) {
		return;
	}

	const FIntPoint Size = GetItemSize(Handle);
	RemoveOccupiedCells(*Position, Size, OccupiedCells);
	ItemPositions.Remove(Handle);
}

bool UAuraGridInventoryLayout::IsCellOccupied(FIntPoint Cell) const {
	return OccupiedCells.Contains(Cell);
}

bool UAuraGridInventoryLayout::GetItemPosition(const FAuraItemHandle& Handle,
	FIntPoint& OutPos) const {
	if (const FIntPoint* Found = ItemPositions.Find(Handle)) {
		OutPos = *Found;
		return true;
	}
	return false;
}

void UAuraGridInventoryLayout::GetAllItems(
	TArray<FAuraItemHandle>& OutHandles) const {
	OutHandles.Reset();

	for (const auto& Pair : ItemPositions) {
		OutHandles.Add(Pair.Key);
	}
}

bool UAuraGridInventoryLayout::TryAddItemAt(const FAuraItemHandle& Handle,
	FIntPoint Position) {
	if (!IsSpatialItem(Handle)) {
		return true;
	}

	FIntPoint Size = GetItemSize(Handle);

	if (!CanPlaceItemAt(Position, Size)) {
		return false;
	}

	ItemPositions.Add(Handle, Position);

	AddOccupiedCells(Position, Size, OccupiedCells);
	return true;
}

bool UAuraGridInventoryLayout::CanAddItem(
	const UAuraItemDefinition* ItemDef) const {
	return GetAvailablePlacements(ItemDef) > 0;
}

int32 UAuraGridInventoryLayout::GetAvailablePlacements(
	const UAuraItemDefinition* ItemDef) const {
	if (!ItemDef) {
		return 0;
	}

	if (!IsSpatialItem(ItemDef)) {
		return TNumericLimits<int32>::Max();
	}

	const UAuraItemFragment_Size* SizeFrag =
		ItemDef->FindFragment<UAuraItemFragment_Size>();
	const FIntPoint Size = SizeFrag ? SizeFrag->Size : FIntPoint(1, 1);

	if (Size.X <= 0 || Size.Y <= 0) {
		return 0;
	}

	TSet<FIntPoint> SimulatedOccupiedCells = OccupiedCells;
	int32 PlacementCount = 0;

	while (true) {
		bool bPlaced = false;

		for (int32 Y = 0; Y < Rows && !bPlaced; ++Y) {
			for (int32 X = 0; X < Columns; ++X) {
				const FIntPoint Position(X, Y);

				if (!CanPlaceItemAt(
					Position,
					Size,
					SimulatedOccupiedCells)) {
					continue;
				}

				AddOccupiedCells(
					Position,
					Size,
					SimulatedOccupiedCells);

				++PlacementCount;
				bPlaced = true;
				break;
			}
		}

		if (!bPlaced) {
			break;
		}
	}

	return PlacementCount;
}

bool UAuraGridInventoryLayout::IsSpatialItem(
	const UAuraItemDefinition* ItemDef) const {
	if (!ItemDef) {
		return false;
	}

	const UAuraItemFragment_LayoutBehavior* Behavior =
		ItemDef->FindFragment<UAuraItemFragment_LayoutBehavior>();

	if (!Behavior) {
		return true;
	}

	return Behavior->LayoutBehaviorTag !=
		TAG_AURA_INVENTORY_LAYOUT_NONSPATIAL;
}

void UAuraGridInventoryLayout::AddOccupiedCells(
	FIntPoint Position,
	FIntPoint Size,
	TSet<FIntPoint>& InOccupiedCells) const {
	for (int32 Y = 0; Y < Size.Y; ++Y) {
		for (int32 X = 0; X < Size.X; ++X) {
			InOccupiedCells.Add(Position + FIntPoint(X, Y));
		}
	}
}

void UAuraGridInventoryLayout::RemoveOccupiedCells(
	FIntPoint Position,
	FIntPoint Size,
	TSet<FIntPoint>& InOccupiedCells) const {
	for (int32 Y = 0; Y < Size.Y; ++Y) {
		for (int32 X = 0; X < Size.X; ++X) {
			InOccupiedCells.Remove(Position + FIntPoint(X, Y));
		}
	}
}

bool UAuraGridInventoryLayout::IsSpatialItem(
	const FAuraItemHandle& Handle) const {

	if (!Inventory) {
		return true;
	}

	const FAuraItemInstance* Item =
		Inventory->FindItem(Handle);

	if (!Item) {
		return true;
	}

	const UAuraItemFragment_LayoutBehavior* Behavior =
		Item->FindFragment<UAuraItemFragment_LayoutBehavior>();

	if (!Behavior) {
		return true;
	}

	return Behavior->LayoutBehaviorTag !=
		TAG_AURA_INVENTORY_LAYOUT_NONSPATIAL;
}

bool UAuraGridInventoryLayout::GetItemAtCell(
	FIntPoint Cell,
	FAuraItemHandle& OutHandle) const {
	for (const auto& Pair : ItemPositions) {
		const FAuraItemHandle& Handle = Pair.Key;
		const FIntPoint& Position = Pair.Value;

		const FIntPoint Size = GetItemSize(Handle);

		if (Cell.X >= Position.X &&
			Cell.X < Position.X + Size.X &&
			Cell.Y >= Position.Y &&
			Cell.Y < Position.Y + Size.Y) {
			OutHandle = Handle;
			return true;
		}
	}

	return false;
}
