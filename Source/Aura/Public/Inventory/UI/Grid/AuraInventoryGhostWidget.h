#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "AuraInventoryGhostWidget.generated.h"

UENUM(BlueprintType)
enum class EGhostWidgetState : uint8 { Valid, Invalid };

UCLASS()
class AURA_API UAuraInventoryGhostWidget : public UUserWidget {
  GENERATED_BODY()

 public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost|Config")
  FLinearColor ValidColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.5f);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost|Config")
  FLinearColor InvalidColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.5f);

  UFUNCTION(BlueprintNativeEvent, Category = "Ghost|Logic")
  void SetGhostState(EGhostWidgetState NewState);

  virtual void SetGhostState_Implementation(EGhostWidgetState NewState);
};