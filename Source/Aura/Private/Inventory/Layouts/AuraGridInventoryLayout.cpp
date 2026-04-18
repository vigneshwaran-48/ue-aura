#include "Inventory/Layouts/AuraGridInventoryLayout.h"

#include "Inventory/AuraInventoryComponent.h"
#include "Inventory/AuraItemInstance.h"
#include "Inventory/Fragments/AuraItemFragment_Size.h"

FIntPoint UAuraGridInventoryLayout::GetItemSize(
    const FAuraItemHandle& Handle) const {
  const FAuraItemInstance* Item = Inventory->FindItem(Handle);
  if (!Item) return FIntPoint(1, 1);

  const UAuraItemFragment_Size* Frag =
      Item->FindFragment<UAuraItemFragment_Size>();
  return Frag ? Frag->Size : FIntPoint(1, 1);
}

bool UAuraGridInventoryLayout::TryAddItem(const FAuraItemHandle& Handle) {
  const FIntPoint Size = GetItemSize(Handle);

  for (int y = 0; y < Rows; y++) {
    for (int x = 0; x < Columns; x++) {
      FIntPoint Pos(x, y);

      if (CanPlaceItemAt(Pos, Size)) {
        ItemPositions.Add(Handle, Pos);

        for (int dy = 0; dy < Size.Y; dy++)
          for (int dx = 0; dx < Size.X; dx++)
            OccupiedCells.Add(Pos + FIntPoint(dx, dy));

        return true;
      }
    }
  }

  return false;
}

bool UAuraGridInventoryLayout::CanPlaceItemAt(FIntPoint Position,
                                              FIntPoint Size) const {
  if (Position.X < 0 || Position.Y < 0) return false;
  if (Position.X + Size.X > Columns || Position.Y + Size.Y > Rows) return false;

  for (int y = 0; y < Size.Y; y++)
    for (int x = 0; x < Size.X; x++)
      if (OccupiedCells.Contains(Position + FIntPoint(x, y))) return false;

  return true;
}

void UAuraGridInventoryLayout::RemoveItem(const FAuraItemHandle& Handle) {
  if (!ItemPositions.Contains(Handle)) return;

  FIntPoint Pos = ItemPositions[Handle];
  FIntPoint Size = GetItemSize(Handle);

  for (int y = 0; y < Size.Y; y++)
    for (int x = 0; x < Size.X; x++)
      OccupiedCells.Remove(Pos + FIntPoint(x, y));

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