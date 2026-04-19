#pragma once

#include "Blueprint/DragDropOperation.h"
#include "CoreMinimal.h"
#include "Inventory/AuraItemHandle.h"
#include "AuraInventoryDragDropOperation.generated.h"

UCLASS()
class AURA_API UAuraInventoryDragDropOperation : public UDragDropOperation {
  GENERATED_BODY()

 public:
  UPROPERTY()
  FAuraItemHandle ItemHandle;

  UPROPERTY()
  FIntPoint ItemSize;

  UPROPERTY()
  TObjectPtr<UUserWidget> SourceWidget;

  UPROPERTY()
  FIntPoint OriginalPosition;
};