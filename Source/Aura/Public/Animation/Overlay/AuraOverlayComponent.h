#pragma once

#include "AuraOverlayDefinition.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "AuraOverlayComponent.generated.h"

UCLASS(ClassGroup = (Aura), meta = (BlueprintSpawnableComponent))
class AURA_API UAuraOverlayComponent : public UActorComponent {
  GENERATED_BODY()

 public:
  UPROPERTY(Transient, BlueprintReadOnly, Category = "Aura|Overlay")
  TObjectPtr<const UAuraOverlayDefinition> BaseOverlay;

  UPROPERTY(Transient, BlueprintReadOnly, Category = "Aura|Overlay")
  TObjectPtr<const UAuraOverlayDefinition> StatusOverlay;

  UPROPERTY(Transient, BlueprintReadOnly, Category = "Aura|Overlay")
  TArray<TObjectPtr<const UAuraOverlayDefinition>> EquipmentOverlays;

 public:
  UFUNCTION(BlueprintCallable, Category = "Aura|Overlay")
  void SetBaseOverlay(const UAuraOverlayDefinition* NewOverlay);

  UFUNCTION(BlueprintCallable, Category = "Aura|Overlay")
  void SetStatusOverlay(const UAuraOverlayDefinition* NewOverlay);

  UFUNCTION(BlueprintCallable, Category = "Aura|Overlay")
  void AddEquipmentOverlay(const UAuraOverlayDefinition* NewOverlay);

  UFUNCTION(BlueprintCallable, Category = "Aura|Overlay")
  void RemoveEquipmentOverlayBySlot(FGameplayTag SlotTag);

 private:
  bool DoesOverlayUseSlot(const UAuraOverlayDefinition* Overlay,
                          const FGameplayTag& SlotTag) const;
};