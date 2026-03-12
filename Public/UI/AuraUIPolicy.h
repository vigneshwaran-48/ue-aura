#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AuraUIPolicy.generated.h"

class UAuraPrimaryGameLayout;

UCLASS()
class AURA_API UAuraUIPolicy : public UObject {
  GENERATED_BODY()

 public:
  void Initialize(APlayerController* PC);

  UAuraPrimaryGameLayout* GetRootLayout() const;

 private:
  UPROPERTY()
  UAuraPrimaryGameLayout* RootLayout;

  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<UAuraPrimaryGameLayout> LayoutClass;
};