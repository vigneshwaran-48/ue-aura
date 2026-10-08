#include "UI/AuraUIContextSubsystem.h"

void UAuraUIContextSubsystem::SetContext(
    FGameplayTag UITag,
    const FAuraUIContext& Context)
{
    if (!UITag.IsValid())
        return;

    Contexts.Add(UITag, Context);
}

bool UAuraUIContextSubsystem::ConsumeContext(
    FGameplayTag UITag,
    FAuraUIContext& OutContext)
{
    if (!UITag.IsValid())
        return false;

    FAuraUIContext* FoundContext = Contexts.Find(UITag);

    if (!FoundContext)
        return false;

    OutContext = *FoundContext;
    Contexts.Remove(UITag);

    return true;
}

bool UAuraUIContextSubsystem::HasContext(FGameplayTag UITag) const
{
    return Contexts.Contains(UITag);
}

void UAuraUIContextSubsystem::ClearContext(FGameplayTag UITag)
{
    Contexts.Remove(UITag);
}