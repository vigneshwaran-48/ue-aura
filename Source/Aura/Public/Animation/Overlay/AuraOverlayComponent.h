#pragma once

#include "AuraOverlayDefinition.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "AuraOverlayComponent.generated.h"

class USkeletalMeshComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AURA_API UAuraOverlayComponent : public UActorComponent {
  GENERATED_BODY()

 public:
  UAuraOverlayComponent();

 protected:
  virtual void BeginPlay() override;

 public:
  UPROPERTY(EditDefaultsOnly, Category = "Overlay")
  const UAuraOverlayDefinition* BaseOverlayDefault = nullptr;

  UPROPERTY(EditDefaultsOnly, Category = "Overlay")
  const UAuraOverlayDefinition* PoseOverlayDefault = nullptr;

 private:
  UPROPERTY()
  USkeletalMeshComponent* OwnerMesh = nullptr;

 public:
  void InitializeOverlay();

  void SetBaseOverlay(const UAuraOverlayDefinition* NewOverlay);

  void SetPoseOverlay(const UAuraOverlayDefinition* NewOverlay);

  void ClearPoseOverlay();

 protected:
  UPROPERTY()
  const UAuraOverlayDefinition* BaseOverlay = nullptr;

  UPROPERTY()
  const UAuraOverlayDefinition* PoseOverlay = nullptr;

 private:
  void ApplyOverlay(const UAuraOverlayDefinition* Overlay);
};