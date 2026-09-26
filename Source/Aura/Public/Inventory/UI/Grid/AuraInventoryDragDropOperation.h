#pragma once

#include "Blueprint/DragDropOperation.h"
#include "CoreMinimal.h"
#include "Inventory/AuraItemHandle.h"
#include "AuraInventoryDragDropOperation.generated.h"

class UAuraInventoryInteractController;

UCLASS()
class AURA_API UAuraInventoryDragDropOperation : public UDragDropOperation
{
    GENERATED_BODY()

public:
    UPROPERTY()
    FAuraItemHandle ItemHandle;

    UPROPERTY()
    FVector2D DragOffset;

    UPROPERTY()
    TObjectPtr<UAuraInventoryInteractController> InteractionController;
};