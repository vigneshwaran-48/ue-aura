#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "AuraInventorySlotWidget.generated.h"

class USizeBox;

UCLASS()
class AURA_API UAuraInventorySlotWidget : public UUserWidget {
  GENERATED_BODY()

 public:
  void SetSlotSize(float InSize);

  void SetSelected(bool bSelected);

 protected:
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<USizeBox> RootSizeBox;

  UPROPERTY(meta = (BindWidgetOptional))
  TObjectPtr<UWidget> SelectionHighlight;
};