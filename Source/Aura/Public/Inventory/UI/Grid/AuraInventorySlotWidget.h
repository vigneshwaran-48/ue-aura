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

 protected:
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<USizeBox> RootSizeBox;
};