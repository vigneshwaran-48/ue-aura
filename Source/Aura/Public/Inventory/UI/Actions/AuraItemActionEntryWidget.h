#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "AuraItemActionEntryWidget.generated.h"

class UCommonTextBlock;

UCLASS()
class AURA_API UAuraItemActionEntryWidget : public UCommonButtonBase
{
    GENERATED_BODY()

public:
    void SetActionName(const FText& InActionName);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> ActionNameText;
};