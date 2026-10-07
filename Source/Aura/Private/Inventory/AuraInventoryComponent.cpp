#include "Inventory/AuraInventoryComponent.h"

#include "Inventory/AuraItemDefinition.h"
#include "Inventory/Layouts/AuraInventoryLayout.h"

void UAuraInventoryComponent::BeginPlay() {
	Super::BeginPlay();

	if (Layout) {
		Layout->Initialize(this);
	}
}

FAuraInventoryAddResult
UAuraInventoryComponent::AddItem(
	const UAuraItemDefinition* ItemDef,
	int32 Quantity) {

	FAuraInventoryAddResult Result;

	if (!ItemDef || Quantity <= 0) {
		return Result;
	}

	if (!Layout) {
		UE_LOG(LogTemp, Warning, TEXT("Invalid layout definition!"));
		Result.QuantityRemaining = Quantity;
		return Result;
	}

	Result.QuantityRemaining = Quantity;

	// First fill existing compatible stacks.
	if (ItemDef->bStackable) {
		for (auto& Pair : Items) {
			if (Result.QuantityRemaining <= 0) {
				break;
			}

			FAuraItemInstance& Item = Pair.Value;

			if (!Item.Definition ||
				Item.Definition->ItemTag != ItemDef->ItemTag ||
				!Item.Definition->bStackable) {
				continue;
			}

			const int32 AvailableSpace =
				ItemDef->MaxStackSize - Item.StackCount;

			if (AvailableSpace <= 0) {
				continue;
			}

			const int32 QuantityToAdd =
				FMath::Min(Result.QuantityRemaining, AvailableSpace);

			Item.StackCount += QuantityToAdd;

			FAuraItemHandle Handle;
			Handle.Id = Pair.Key;

			FAuraInventoryStackChange& Change =
				Result.StackChanges.AddDefaulted_GetRef();
			Change.ItemHandle = Handle;
			Change.QuantityAdded = QuantityToAdd;
			Change.bCreated = false;

			Result.QuantityAdded += QuantityToAdd;
			Result.QuantityRemaining -= QuantityToAdd;
		}
	}

	// Create new stacks for anything that could not fit into existing stacks.
	while (Result.QuantityRemaining > 0) {
		const int32 StackSize = ItemDef->bStackable
			? FMath::Min(Result.QuantityRemaining, ItemDef->MaxStackSize)
			: 1;

		const int32 Id = NextId++;

		FAuraItemHandle Handle;
		Handle.Id = Id;

		FAuraItemInstance& Item = Items.Add(Id);
		Item.Definition = ItemDef;
		Item.StackCount = StackSize;

		if (Layout->TryAddItem(Handle)) {
			FAuraInventoryStackChange& Change =
				Result.StackChanges.AddDefaulted_GetRef();
			Change.ItemHandle = Handle;
			Change.QuantityAdded = StackSize;
			Change.bCreated = true;

			Result.QuantityAdded += StackSize;
			Result.QuantityRemaining -= StackSize;
			continue;
		}

		Items.Remove(Id);
		break;
	}

	return Result;
}

const FAuraItemInstance*
UAuraInventoryComponent::FindItem(const FAuraItemHandle& Handle) const {
	return Items.Find(Handle.Id);
}

FAuraItemInstance*
UAuraInventoryComponent::FindItem(const FAuraItemHandle& Handle) {
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

bool UAuraInventoryComponent::RemoveItem(
	const FAuraItemHandle& Handle) {
	if (!Handle.IsValid()) {
		return false;
	}

	if (!Items.Contains(Handle.Id)) {
		return false;
	}

	if (Layout) {
		Layout->RemoveItem(Handle);
	}

	Items.Remove(Handle.Id);
	return true;
}

bool UAuraInventoryComponent::RemoveItemQuantity(
	const FAuraItemHandle& Handle,
	int32 Quantity) {
	if (!Handle.IsValid() || Quantity <= 0) {
		return false;
	}

	FAuraItemInstance* Item = Items.Find(Handle.Id);
	if (!Item || !Item->Definition) {
		return false;
	}

	if (Quantity >= Item->StackCount) {
		return RemoveItem(Handle);
	}

	Item->StackCount -= Quantity;
	return true;
}

bool UAuraInventoryComponent::CanAddItem(
	const UAuraItemDefinition* ItemDef,
	int32 Quantity) const {

	if (!ItemDef || Quantity <= 0) {
		return false;
	}

	if (!Layout) {
		return false;
	}

	int32 Remaining = Quantity;

	if (ItemDef->bStackable) {
		for (const auto& Pair : Items) {
			const FAuraItemInstance& Item = Pair.Value;

			if (!Item.Definition ||
				Item.Definition->ItemTag != ItemDef->ItemTag ||
				!Item.Definition->bStackable) {
				continue;
			}

			const int32 AvailableSpace =
				ItemDef->MaxStackSize - Item.StackCount;

			if (AvailableSpace <= 0) {
				continue;
			}

			Remaining -= AvailableSpace;

			if (Remaining <= 0) {
				return true;
			}
		}
	}

	const int32 StackSize = ItemDef->bStackable
		? ItemDef->MaxStackSize
		: 1;

	if (StackSize <= 0) {
		return false;
	}

	const int32 RequiredNewStacks =
		FMath::DivideAndRoundUp(Remaining, StackSize);

	return Layout->GetAvailablePlacements(ItemDef) >= RequiredNewStacks;
}

bool UAuraInventoryComponent::CombineItems(
	const FAuraItemHandle& SourceHandle,
	const FAuraItemHandle& TargetHandle) {

	if (!SourceHandle.IsValid() ||
		!TargetHandle.IsValid() ||
		SourceHandle == TargetHandle) {
		return false;
	}

	FAuraItemInstance* SourceItem =
		Items.Find(SourceHandle.Id);

	FAuraItemInstance* TargetItem =
		Items.Find(TargetHandle.Id);

	if (!SourceItem ||
		!TargetItem ||
		!SourceItem->Definition ||
		!TargetItem->Definition) {
		return false;
	}

	const UAuraItemDefinition* SourceDefinition =
		SourceItem->Definition;

	const UAuraItemDefinition* TargetDefinition =
		TargetItem->Definition;

	if (!SourceDefinition->bStackable ||
		!TargetDefinition->bStackable) {
		return false;
	}

	if (SourceDefinition->ItemTag != TargetDefinition->ItemTag) {
		return false;
	}

	const int32 AvailableSpace =
		TargetDefinition->MaxStackSize - TargetItem->StackCount;

	if (AvailableSpace <= 0) {
		return false;
	}

	const int32 QuantityToTransfer =
		FMath::Min(
			SourceItem->StackCount,
			AvailableSpace);

	if (QuantityToTransfer <= 0) {
		return false;
	}

	SourceItem->StackCount -= QuantityToTransfer;
	TargetItem->StackCount += QuantityToTransfer;

	if (SourceItem->StackCount <= 0) {
		RemoveItem(SourceHandle);
	}

	return true;
}