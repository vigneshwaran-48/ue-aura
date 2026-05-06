#include "Inventory/AuraItemDefinition.h"
#include "Misc/DataValidation.h"

#if WITH_EDITOR

EDataValidationResult
UAuraItemDefinition::IsDataValid(FDataValidationContext &Context) const {
  if (!ItemTag.IsValid()) {
    Context.AddError(
        NSLOCTEXT("Aura", "InvalidItemTag", "ItemTag must be valid."));
    return EDataValidationResult::Invalid;
  }

  return Super::IsDataValid(Context);
}

#endif
