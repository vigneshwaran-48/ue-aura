#include "Animation/Overlay/AuraOverlayDefinition.h"

#include "Animation/Overlay/AuraOverlayTypes.h"

#if WITH_EDITOR
#include "GameplayTagContainer.h"
#include "Misc/DataValidation.h"

EDataValidationResult UAuraOverlayDefinition::IsDataValid(
    FDataValidationContext& Context) const {
  Super::IsDataValid(Context);

  auto ValidateSet = [&](const FAuraOverlayMovementSet& Set) {
    auto ValidateAnim = [&](const FAuraWeightedAnim& Anim) {
      for (const FAuraOverlaySlotWeight& SlotWeight : Anim.SlotWeights) {
        if (!Slots.HasTagExact(SlotWeight.Slot)) {
          Context.AddError(FText::FromString(
              FString::Printf(TEXT("Invalid Slot '%s' not present in Slots"),
                              *SlotWeight.Slot.ToString())));
        }
      }
    };

    ValidateAnim(Set.Idle);
    ValidateAnim(Set.Walk);
    ValidateAnim(Set.Run);
    ValidateAnim(Set.Sprint);
  };

  ValidateSet(PoseSet.Stand.Relax);
  ValidateSet(PoseSet.Stand.Ready);
  ValidateSet(PoseSet.Stand.Aim);
  ValidateSet(PoseSet.Crouch.Relax);
  ValidateSet(PoseSet.Crouch.Ready);
  ValidateSet(PoseSet.Crouch.Aim);

  return Context.GetNumErrors() > 0 ? EDataValidationResult::Invalid
                                    : EDataValidationResult::Valid;
}
#endif