#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Inventory/AuraItemHandle.h"
#include "Inventory/AuraItemInstance.h"
#include "AuraInventoryComponent.generated.h"

class UAuraInventoryLayout;
class UAuraItemDefinition;

USTRUCT(BlueprintType)
struct FAuraInventoryStackChange {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FAuraItemHandle ItemHandle;

	UPROPERTY(BlueprintReadOnly)
	int32 QuantityAdded = 0;

	UPROPERTY(BlueprintReadOnly)
	bool bCreated = false;
};

USTRUCT(BlueprintType)
struct FAuraInventoryAddResult {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FAuraInventoryStackChange> StackChanges;

	UPROPERTY(BlueprintReadOnly)
	int32 QuantityAdded = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 QuantityRemaining = 0;
};

UCLASS(ClassGroup = (Aura), meta = (BlueprintSpawnableComponent))
class AURA_API UAuraInventoryComponent : public UActorComponent {
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FAuraInventoryAddResult AddItem(const UAuraItemDefinition* ItemDef, int32 Quantity = 1);

	const FAuraItemInstance* FindItem(const FAuraItemHandle& Handle) const;
	FAuraItemInstance* FindItem(const FAuraItemHandle& Handle);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool GetItem(const FAuraItemHandle& Handle, FAuraItemInstance& OutItem) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UAuraInventoryLayout* GetLayout() const { return Layout; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItemWithTag(FGameplayTag ItemTag) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItemDefinition(const UAuraItemDefinition* ItemDef) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FAuraItemHandle FindFirstItemByTag(FGameplayTag ItemTag) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FAuraItemHandle
		FindFirstItemByDefinition(const UAuraItemDefinition* ItemDef) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FAuraItemHandle> FindItemsByTag(FGameplayTag ItemTag) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FAuraItemHandle>
		FindItemsByDefinition(const UAuraItemDefinition* ItemDef) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FAuraItemHandle> GetAllItemHandles() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(const FAuraItemHandle& Handle);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemQuantity(const FAuraItemHandle& Handle, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool CanAddItem(const UAuraItemDefinition* ItemDef, int32 Quantity = 1) const;

	UFUNCTION(BlueprintCallable)
	bool CombineItems(const FAuraItemHandle& SourceHandle, const FAuraItemHandle& TargetHandle);

private:
	UPROPERTY()
	TMap<int32, FAuraItemInstance> Items;

	int32 NextId = 0;

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UAuraInventoryLayout> Layout;
};
