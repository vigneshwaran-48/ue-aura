#pragma once

#include "AuraOverlayDefinition.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "AuraOverlayComponent.generated.h"

UCLASS(ClassGroup = (Aura), meta = (BlueprintSpawnableComponent))
class AURA_API UAuraOverlayComponent : public UActorComponent {
  GENERATED_BODY()

 public:

  UPROPERTY(Transient)
  TObjectPtr<const UAuraOverlayDefinition> BaseOverlay;

  UPROPERTY(Transient)
  TObjectPtr<const UAuraOverlayDefinition> StatusOverlay;

  UPROPERTY(Transient)
  TArray<TObjectPtr<const UAuraOverlayDefinition>> EquipmentOverlays;

 public:
  void SetBaseOverlay(const UAuraOverlayDefinition* NewOverlay);
  void SetStatusOverlay(const UAuraOverlayDefinition* NewOverlay);

  void AddEquipmentOverlay(const UAuraOverlayDefinition* NewOverlay);
  void RemoveEquipmentOverlayBySlot(FGameplayTag SlotTag);

 private:
  bool DoesOverlayUseSlot(const UAuraOverlayDefinition* Overlay,
                          const FGameplayTag& SlotTag) const;
};