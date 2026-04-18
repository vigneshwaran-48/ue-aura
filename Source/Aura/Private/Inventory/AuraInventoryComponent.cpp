#include "Inventory/AuraInventoryComponent.h"

#include "Inventory/AuraItemDefinition.h"
#include "Inventory/Layouts/AuraInventoryLayout.h"

void UAuraInventoryComponent::BeginPlay() {
  Super::BeginPlay();

  if (Layout) {
    Layout->Initialize(this);
  }
}

FAuraItemHandle UAuraInventoryComponent::AddItem(
    const UAuraItemDefinition* ItemDef) {
  FAuraItemHandle Handle;

  if (!ItemDef || !Layout) return Handle;

  const int32 Id = NextId++;
  Handle.Id = Id;

  FAuraItemInstance& Item = Items.Add(Id);
  Item.Definition = ItemDef;

  if (Layout->TryAddItem(Handle)) {
    UE_LOG(LogTemp, Warning, TEXT("Item added to layout"));
    return Handle;
  }

  // rollback
  Items.Remove(Id);
  return FAuraItemHandle();
}

const FAuraItemInstance* UAuraInventoryComponent::FindItem(
    const FAuraItemHandle& Handle) const {
  return Items.Find(Handle.Id);
}

FAuraItemInstance* UAuraInventoryComponent::FindItem(
    const FAuraItemHandle& Handle) {
  return Items.Find(Handle.Id);
}

bool UAuraInventoryComponent::GetItem(const FAuraItemHandle& Handle,
                                      FAuraItemInstance& OutItem) const {
  if (const FAuraItemInstance* Found = FindItem(Handle)) {
    OutItem = *Found;
    return true;
  }
  return false;
}