#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AuraUIPolicy.generated.h"

class UAuraPrimaryGameLayout;
class APlayerController;

UCLASS()
class AURA_API UAuraUIPolicy : public UObject {
  GENERATED_BODY()

 public:
  void Initialize(APlayerController* PlayerController,
                  TSubclassOf<UAuraPrimaryGameLayout> LayoutClass);

  UAuraPrimaryGameLayout* GetRootLayout() const;

 private:
  UPROPERTY()
  TObjectPtr<UAuraPrimaryGameLayout> RootLayout;
};