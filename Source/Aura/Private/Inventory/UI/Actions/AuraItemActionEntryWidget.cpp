#include "Inventory/UI/Actions/AuraItemActionEntryWidget.h"

#include "CommonTextBlock.h"

void UAuraItemActionEntryWidget::SetActionName(
    const FText& InActionName)
{
    if (ActionNameText)
        ActionNameText->SetText(InActionName);
}