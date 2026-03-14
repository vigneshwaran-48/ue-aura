#include "UI/AuraUISubsystem.h"

#include "Core/AuraGameInstance.h"
#include "UI/AuraPrimaryGameLayout.h"
#include "UI/AuraUIConfig.h"
#include "UI/AuraUIPolicy.h"

void UAuraUISubsystem::InitializeUI(APlayerController* PC) {
  if (!PC) return;

  UAuraGameInstance* GI = Cast<UAuraGameInstance>(GetGameInstance());

  if (!GI || !GI->UIConfig) return;

  UIPolicy = NewObject<UAuraUIPolicy>(this);

  UIPolicy->Initialize(PC, GI->UIConfig->PrimaryGameLayoutClass);
}

UAuraPrimaryGameLayout* UAuraUISubsystem::GetRootLayout() const {
  return UIPolicy ? UIPolicy->GetRootLayout() : nullptr;
}

void UAuraUISubsystem::Initialize(FSubsystemCollectionBase& Collection) {
  Super::Initialize(Collection);

  UWorld* World = GetWorld();

  if (!World) {
    return;
  }

  World->GetTimerManager().SetTimer(
      UIInitTimerHandle, this, &UAuraUISubsystem::TryInitializeUI, 0.1f, true);
}

void UAuraUISubsystem::TryInitializeUI() {
  UWorld* World = GetWorld();
  if (!World) return;

  if (UIPolicy) {
    World->GetTimerManager().ClearTimer(UIInitTimerHandle);
    return;
  }

  APlayerController* PC = World->GetFirstPlayerController();
  if (!PC) return;

  UAuraGameInstance* GI = Cast<UAuraGameInstance>(GetGameInstance());
  if (!GI || !GI->UIConfig) return;

  UIPolicy = NewObject<UAuraUIPolicy>(this);

  UIPolicy->Initialize(PC, GI->UIConfig->PrimaryGameLayoutClass);

  World->GetTimerManager().ClearTimer(UIInitTimerHandle);

  UE_LOG(LogTemp, Warning, TEXT("Aura UI Initialized"));

  for (const FAuraUIPushRequest& Request : PendingPushRequests) {
    if (UIPolicy && UIPolicy->GetRootLayout()) {
      UIPolicy->GetRootLayout()->PushWidgetToLayer(Request.LayerTag,
                                                   Request.WidgetClass);
    }
  }

  PendingPushRequests.Empty();
}

UCommonActivatableWidget* UAuraUISubsystem::PushWidgetToLayer(
    FGameplayTag LayerTag, TSubclassOf<UCommonActivatableWidget> WidgetClass) {
  if (!UIPolicy) {
    FAuraUIPushRequest Request;
    Request.LayerTag = LayerTag;
    Request.WidgetClass = WidgetClass;

    PendingPushRequests.Add(Request);

    return nullptr;
  }

  UAuraPrimaryGameLayout* RootLayout = UIPolicy->GetRootLayout();

  if (!RootLayout) {
    return nullptr;
  }

  return RootLayout->PushWidgetToLayer(LayerTag, WidgetClass);
}