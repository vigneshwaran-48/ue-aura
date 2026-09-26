#pragma once

#include "UI/AuraActivatableWidget.h"
#include "AuraGridInventoryScreenWidget.generated.h"

class UAuraInventoryGridWidget;

UCLASS()
class AURA_API UAuraGridInventoryScreenWidget : public UAuraActivatableWidget
{
	GENERATED_BODY()

public:
	virtual bool NativeOnDrop(
		const FGeometry& InGeometry,
		const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAuraInventoryGridWidget> InventoryGrid;
};