#include "Inventory/UI/Actions/AuraItemActionMenuWidget.h"

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Inventory/UI/Actions/AuraItemActionEntryWidget.h"
#include "Input/CommonUIInputTypes.h"
#include "Framework/Application/SlateApplication.h"

void UAuraItemActionMenuWidget::SetActions(
    const FAuraItemHandle& InItemHandle,
    const TArray<FAuraItemAction>& InActions)
{
    ItemHandle = InItemHandle;
    Actions = InActions;

    if (!ActionList || !ActionEntryClass)
        return;

    ActionList->ClearChildren();
    ActionEntries.Empty();

    for (int32 Index = 0; Index < Actions.Num(); ++Index)
    {
        UAuraItemActionEntryWidget* Entry =
            CreateWidget<UAuraItemActionEntryWidget>(
                GetOwningPlayer(),
                ActionEntryClass);

        if (!Entry)
            continue;

        Entry->SetActionName(Actions[Index].DisplayName);

        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Action entry %d: DesiredSize=(%.1f, %.1f)"),
            Index,
            Entry->GetDesiredSize().X,
            Entry->GetDesiredSize().Y);

        Entry->OnClicked().AddLambda(
            [this, Index]()
            {
                HandleActionClicked(Index);
            });

        ActionList->AddChildToVerticalBox(Entry);
        ActionEntries.Add(Entry);
    }

    SelectedActionIndex =
        ActionEntries.IsEmpty() ? INDEX_NONE : 0;

    ForceLayoutPrepass();
}

void UAuraItemActionMenuWidget::NativeOnActivated()
{
    Super::NativeOnActivated();

    RegisterAction(
        MoveUpInputActionData,
        FSimpleDelegate::CreateUObject(
            this,
            &ThisClass::MoveSelection,
            -1));

    RegisterAction(
        MoveDownInputActionData,
        FSimpleDelegate::CreateUObject(
            this,
            &ThisClass::MoveSelection,
            1));

    RegisterAction(
        CancelInputActionData,
        FSimpleDelegate::CreateUObject(
            this,
            &ThisClass::CancelMenu));

    RegisterAction(
        ConfirmInputActionData,
        FSimpleDelegate::CreateUObject(
            this,
            &ThisClass::ConfirmSelection));

    FocusSelectedAction();
}

void UAuraItemActionMenuWidget::NativeOnDeactivated()
{
    Super::NativeOnDeactivated();
}

void UAuraItemActionMenuWidget::RegisterAction(
    const FDataTableRowHandle& ActionHandle,
    const FSimpleDelegate& Delegate)
{
    if (ActionHandle.IsNull())
        return;

    FBindUIActionArgs BindArgs(ActionHandle, Delegate);
    BindArgs.bIsPersistent = true;
    BindArgs.bDisplayInActionBar = false;

    RegisterUIActionBinding(BindArgs);
}

void UAuraItemActionMenuWidget::MoveSelection(int32 Direction)
{
    UE_LOG(LogTemp, Warning,
        TEXT("Before: Index=%d, Direction=%d, Count=%d"),
        SelectedActionIndex, Direction, ActionEntries.Num());
    if (ActionEntries.IsEmpty())
        return;

    SelectedActionIndex = (SelectedActionIndex + Direction
        + ActionEntries.Num()) % ActionEntries.Num();

    for (int32 Index = 0; Index < ActionEntries.Num(); ++Index)
    {
        if (UAuraItemActionEntryWidget* Entry = ActionEntries[Index])
        {
            Entry->ClearSelection();
        }
    }

    UE_LOG(LogTemp, Warning,
        TEXT("After: Index=%d"), SelectedActionIndex);

    FocusSelectedAction();
}

void UAuraItemActionMenuWidget::FocusSelectedAction()
{
    if (!ActionEntries.IsValidIndex(SelectedActionIndex))
        return;

    if (ActionEntries[SelectedActionIndex])
        ActionEntries[SelectedActionIndex]->SetIsSelected(true);

    for (int32 Index = 0; Index < ActionEntries.Num(); ++Index)
    {
        if (ActionEntries[Index])
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("Entry %d: HasKeyboardFocus=%d, IsSelected=%d"),
                Index,
                ActionEntries[Index]->HasKeyboardFocus(),
                Index == SelectedActionIndex);
        }
    }
}

void UAuraItemActionMenuWidget::CancelMenu()
{
    DeactivateWidget();
    OnMenuClosed.ExecuteIfBound();
}

void UAuraItemActionMenuWidget::HandleActionClicked(
    int32 ActionIndex)
{
    if (!Actions.IsValidIndex(ActionIndex))
        return;

    const FAuraItemHandle SelectedItem = ItemHandle;
    const FAuraItemAction SelectedAction = Actions[ActionIndex];

    DeactivateWidget();

    OnActionSelected.ExecuteIfBound(
        SelectedItem,
        SelectedAction);
}

void UAuraItemActionMenuWidget::ConfirmSelection()
{
    if (!Actions.IsValidIndex(SelectedActionIndex))
        return;

    HandleActionClicked(SelectedActionIndex);
}