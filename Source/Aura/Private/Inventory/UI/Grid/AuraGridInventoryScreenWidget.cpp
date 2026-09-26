#include "Inventory/UI/Grid/AuraGridInventoryScreenWidget.h"

#include "Inventory/UI/Grid/AuraInventoryGridWidget.h"

bool UAuraGridInventoryScreenWidget::NativeOnDrop(
	const FGeometry& InGeometry,
	const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (InventoryGrid &&
		InventoryGrid->HandleDropOutsideGrid()) {
		return true;
	}

	return Super::NativeOnDrop(
		InGeometry,
		InDragDropEvent,
		InOperation);
}