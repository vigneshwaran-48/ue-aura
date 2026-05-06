#include "Inventory/AuraInventoryComponent.h"

#include "Inventory/AuraItemDefinition.h"
#include "Inventory/Layouts/AuraInventoryLayout.h"

void UAuraInventoryComponent::BeginPlay() {
  Super::BeginPlay();

  if (Layout) {
    Layout->Initialize(this);
  }
}

FAuraItemHandle
UAuraInventoryComponent::AddItem(const UAuraItemDefinition *ItemDef) {
  FAuraItemHandle Handle;

  if (!ItemDef) {
    UE_LOG(LogTemp, Warning, TEXT("Invalid item definition!"));
    return Handle;
  }
  if (!Layout) {
    UE_LOG(LogTemp, Warning, TEXT("Invalid layout definition!"));
    return Handle;
  }

  const int32 Id = NextId++;
  Handle.Id = Id;

  FAuraItemInstance &Item = Items.Add(Id);
  Item.Definition = ItemDef;

  if (Layout->TryAddItem(Handle)) {
    UE_LOG(LogTemp, Warning, TEXT("Item added to layout"));
    return Handle;
  }

  // rollback
  UE_LOG(
      LogTemp, Warning,
      TEXT("Unable to add item to layout, Reverting the item from inventory"));
  Items.Remove(Id);
  return FAuraItemHandle();
}

const FAuraItemInstance *
UAuraInventoryComponent::FindItem(const FAuraItemHandle &Handle) const {
  return Items.Find(Handle.Id);
}

FAuraItemInstance *
UAuraInventoryComponent::FindItem(const FAuraItemHandle &Handle) {
  return Items.Find(Handle.Id);
}

bool UAuraInventoryComponent::GetItem(const FAuraItemHandle &Handle,
                                      FAuraItemInstance &OutItem) const {
  if (const FAuraItemInstance *Found = FindItem(Handle)) {
    OutItem = *Found;
    return true;
  }
  return false;
}

bool UAuraInventoryComponent::HasItemWithTag(
    FGameplayTag ItemTag) const {
  if (!ItemTag.IsValid()) {
    return false;
  }

  for (const auto& Pair : Items) {
    const FAuraItemInstance& Item = Pair.Value;

    if (Item.Definition &&
        Item.Definition->ItemTag.MatchesTag(ItemTag)) {
      return true;
    }
  }

  return false;
}

bool UAuraInventoryComponent::HasItemDefinition(
    const UAuraItemDefinition* ItemDef) const {
  if (!ItemDef) {
    return false;
  }

  for (const auto& Pair : Items) {
    if (Pair.Value.Definition == ItemDef) {
      return true;
    }
  }

  return false;
}

FAuraItemHandle UAuraInventoryComponent::FindFirstItemByTag(
    FGameplayTag ItemTag) const {
  FAuraItemHandle Handle;

  if (!ItemTag.IsValid()) {
    return Handle;
  }

  for (const auto& Pair : Items) {
    const FAuraItemInstance& Item = Pair.Value;

    if (Item.Definition &&
        Item.Definition->ItemTag.MatchesTag(ItemTag)) {
      Handle.Id = Pair.Key;
      return Handle;
    }
  }

  return Handle;
}

FAuraItemHandle
UAuraInventoryComponent::FindFirstItemByDefinition(
    const UAuraItemDefinition* ItemDef) const {
  FAuraItemHandle Handle;

  if (!ItemDef) {
    return Handle;
  }

  for (const auto& Pair : Items) {
    if (Pair.Value.Definition == ItemDef) {
      Handle.Id = Pair.Key;
      return Handle;
    }
  }

  return Handle;
}

TArray<FAuraItemHandle>
UAuraInventoryComponent::FindItemsByTag(
    FGameplayTag ItemTag) const {
  TArray<FAuraItemHandle> Result;

  if (!ItemTag.IsValid()) {
    return Result;
  }

  for (const auto& Pair : Items) {
    const FAuraItemInstance& Item = Pair.Value;

    if (Item.Definition &&
        Item.Definition->ItemTag.MatchesTag(ItemTag)) {
      FAuraItemHandle Handle;
      Handle.Id = Pair.Key;

      Result.Add(Handle);
    }
  }

  return Result;
}

TArray<FAuraItemHandle>
UAuraInventoryComponent::FindItemsByDefinition(
    const UAuraItemDefinition* ItemDef) const {
  TArray<FAuraItemHandle> Result;

  if (!ItemDef) {
    return Result;
  }

  for (const auto& Pair : Items) {
    if (Pair.Value.Definition == ItemDef) {
      FAuraItemHandle Handle;
      Handle.Id = Pair.Key;

      Result.Add(Handle);
    }
  }

  return Result;
}

TArray<FAuraItemHandle>
UAuraInventoryComponent::GetAllItemHandles() const {
  TArray<FAuraItemHandle> Result;

  for (const auto& Pair : Items) {
    FAuraItemHandle Handle;
    Handle.Id = Pair.Key;

    Result.Add(Handle);
  }

  return Result;
}
