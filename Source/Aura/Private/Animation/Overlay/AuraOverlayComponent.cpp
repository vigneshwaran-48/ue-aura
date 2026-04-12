#include "Animation/Overlay/AuraOverlayComponent.h"

#include "Animation/Overlay/AuraOverlayDefinition.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

UAuraOverlayComponent::UAuraOverlayComponent() {
  PrimaryComponentTick.bCanEverTick = false;
}

void UAuraOverlayComponent::BeginPlay() {
  Super::BeginPlay();

  AActor* Owner = GetOwner();
  if (Owner) {
    OwnerMesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
  }

  InitializeOverlay();
}

void UAuraOverlayComponent::InitializeOverlay() {
  BaseOverlay = BaseOverlayDefault;
  PoseOverlay = nullptr;

  if (BaseOverlay) {
    ApplyOverlay(BaseOverlay);
  } else if (PoseOverlayDefault) {
    ApplyOverlay(PoseOverlayDefault);
  }
}

void UAuraOverlayComponent::SetBaseOverlay(
    const UAuraOverlayDefinition* NewOverlay) {
  BaseOverlay = NewOverlay ? NewOverlay : BaseOverlayDefault;

  // Only apply if no pose overlay is active
  if (!PoseOverlay) {
    ApplyOverlay(BaseOverlay);
  }
}

void UAuraOverlayComponent::SetPoseOverlay(
    const UAuraOverlayDefinition* NewOverlay) {
  PoseOverlay = NewOverlay ? NewOverlay : PoseOverlayDefault;

  ApplyOverlay(PoseOverlay);
}

void UAuraOverlayComponent::ClearPoseOverlay() {
  PoseOverlay = nullptr;

  if (BaseOverlay) {
    ApplyOverlay(BaseOverlay);
  } else if (BaseOverlayDefault) {
    ApplyOverlay(BaseOverlayDefault);
  }
}

void UAuraOverlayComponent::ApplyOverlay(
    const UAuraOverlayDefinition* Overlay) {
  if (!Overlay || !OwnerMesh) return;

  if (Overlay->OverlayAnimBP) {
    OwnerMesh->LinkAnimClassLayers(Overlay->OverlayAnimBP);
  }

  UE_LOG(LogTemp, Log, TEXT("Overlay Applied: %s"), *GetNameSafe(Overlay));
}