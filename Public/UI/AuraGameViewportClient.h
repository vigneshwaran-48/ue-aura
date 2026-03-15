#pragma once

#include "CommonGameViewportClient.h"
#include "CoreMinimal.h"
#include "AuraGameViewportClient.generated.h"

/**
 * CommonGame Game Viewport Client
 *
 * Must be configured in DefaultEngine.ini:
 *
 * [/Script/Engine.Engine]
 * GameViewportClientClassName=/Script/Aura.AuraGameViewportClient
 */
UCLASS()
class AURA_API UAuraGameViewportClient : public UCommonGameViewportClient {
  GENERATED_BODY()

 public:
  UAuraGameViewportClient(
      const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

  virtual void Init(FWorldContext& WorldContext,
                    UGameInstance* OwningGameInstance,
                    bool bCreateNewAudioDevice) override;
};
