#pragma once

#include "CoreMinimal.h"
#include "UI/AuraActivatableWidget.h"
#include "Inventory/AuraItemHandle.h"
#include "Inventory/Actions/AuraItemAction.h"
#include "AuraItemActionMenuWidget.generated.h"

class UVerticalBox;
class UAuraItemActionEntryWidget;

DECLARE_DELEGATE_TwoParams(
    FAuraItemActionSelected,
    FAuraItemHandle,
    FAuraItemAction
);

UCLASS()
class AURA_API UAuraItemActionMenuWidget
    : public UAuraActivatableWidget
{
    GENERATED_BODY()

public:
    void SetActions(
        const FAuraItemHandle& InItemHandle,
        const TArray<FAuraItemAction>& InActions);

    FAuraItemActionSelected OnActionSelected;

    DECLARE_DELEGATE(FOnMenuClosed);
    FOnMenuClosed OnMenuClosed;

protected:
    virtual void NativeOnActivated() override;
    virtual void NativeOnDeactivated() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UVerticalBox> ActionList;

    UPROPERTY(EditDefaultsOnly, Category = "Actions")
    TSubclassOf<UAuraItemActionEntryWidget> ActionEntryClass;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    FDataTableRowHandle MoveUpInputActionData;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    FDataTableRowHandle MoveDownInputActionData;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    FDataTableRowHandle CancelInputActionData;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    FDataTableRowHandle ConfirmInputActionData;

private:
    UPROPERTY()
    FAuraItemHandle ItemHandle;

    UPROPERTY()
    TArray<FAuraItemAction> Actions;

    UPROPERTY()
    TArray<TObjectPtr<UAuraItemActionEntryWidget>> ActionEntries;

    int32 SelectedActionIndex = INDEX_NONE;

    void RegisterAction(
        const FDataTableRowHandle& ActionHandle,
        const FSimpleDelegate& Delegate);

    void MoveSelection(int32 Direction);
    void CancelMenu();
    void HandleActionClicked(int32 ActionIndex);
    void FocusSelectedAction();
    void ConfirmSelection();
};