#include "Interaction/AuraInteractableBase.h"
#include "Interaction/AuraInteractableComponent.h"
#include "UI/IndicatorSystem/IndicatorDescriptor.h"
#include "UI/IndicatorSystem/AuraIndicatorManagerComponent.h"
#include "Components/SceneComponent.h"

AAuraInteractableBase::AAuraInteractableBase()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractableComponent = CreateDefaultSubobject<UAuraInteractableComponent>(TEXT("InteractableComponent"));
}

void AAuraInteractableBase::BeginPlay()
{
	Super::BeginPlay();

	if (InteractableComponent)
	{
		InteractableComponent->OnFocusGained.AddDynamic(this, &AAuraInteractableBase::OnFocusGained);
		InteractableComponent->OnFocusLost.AddDynamic(this, &AAuraInteractableBase::OnFocusLost);
		InteractableComponent->OnHighlight.AddDynamic(this, &AAuraInteractableBase::OnHighlightGained);
		InteractableComponent->OnDisableHighlight.AddDynamic(this, &AAuraInteractableBase::OnHighlightLost);
		InteractableComponent->OnInteract.AddDynamic(this, &AAuraInteractableBase::OnInteract);
	}
}

USceneComponent* AAuraInteractableBase::GetIndicatorTargetComponent_Implementation() const
{
	return GetRootComponent();
}

void AAuraInteractableBase::OnFocusGained(AActor* Interactor)
{
	OnHighlightLost(Interactor);
	CreateIndicator(Interactor, PromptIndicatorWidgetClass, ActivePromptDescriptor);
}

void AAuraInteractableBase::OnFocusLost(AActor* Interactor)
{
	if (InteractableComponent->IsHighlighted()) {
		OnHighlightGained(Interactor);
	}
	ClearIndicator(Interactor, ActivePromptDescriptor);
}

void AAuraInteractableBase::OnHighlightGained(AActor* Interactor)
{
	CreateIndicator(Interactor, HighlightIndicatorWidgetClass, ActiveHighlightDescriptor);
}

void AAuraInteractableBase::OnHighlightLost(AActor* Interactor)
{
	ClearIndicator(Interactor, ActiveHighlightDescriptor);
}

void AAuraInteractableBase::OnInteract(AActor* Interactor)
{
	UE_LOG(LogTemp, Log, TEXT("Base Interactable!"));
}

UAuraIndicatorManagerComponent* AAuraInteractableBase::GetIndicatorManagerFromInteractor(AActor* Interactor) const
{
	if (Interactor)
	{
		AController* InteractorController = nullptr;

		if (AController* AsController = Cast<AController>(Interactor))
		{
			InteractorController = AsController;
		} else if (const APawn* Pawn = Cast<APawn>(Interactor))
		{
			InteractorController = Pawn->GetController();
		} else
		{
			InteractorController = Interactor->GetInstigatorController();
		}

		if (InteractorController)
		{
			if (UAuraIndicatorManagerComponent* Manager = InteractorController->FindComponentByClass<UAuraIndicatorManagerComponent>())
			{
				return Manager;
			}
		}
	}

	if (CachedIndicatorManager.IsValid())
	{
		return CachedIndicatorManager.Get();
	}

	return nullptr;
}

void AAuraInteractableBase::CreateIndicator(AActor* Interactor, TSubclassOf<UUserWidget> WidgetClass, TObjectPtr<UIndicatorDescriptor>& OutDescriptor)
{
	if (!WidgetClass || OutDescriptor) return;

	UAuraIndicatorManagerComponent* IndicatorManager = GetIndicatorManagerFromInteractor(Interactor);
	if (!IndicatorManager) return;

	// Cache manager reference for EndPlay cleanup
	CachedIndicatorManager = IndicatorManager;

	USceneComponent* TargetComponent = GetIndicatorTargetComponent();
	if (!TargetComponent) return;

	OutDescriptor = NewObject<UIndicatorDescriptor>(this);
	OutDescriptor->SetDataObject(this);
	OutDescriptor->SetSceneComponent(TargetComponent);
	OutDescriptor->SetIndicatorClass(WidgetClass.Get());

	IndicatorManager->AddIndicator(OutDescriptor);
}

void AAuraInteractableBase::ClearIndicator(AActor* Interactor, TObjectPtr<UIndicatorDescriptor>& Descriptor)
{
	if (!Descriptor) return;

	if (UAuraIndicatorManagerComponent* IndicatorManager = GetIndicatorManagerFromInteractor(Interactor))
	{
		IndicatorManager->RemoveIndicator(Descriptor);
	}

	Descriptor = nullptr;

	// Clear cached manager if no active descriptors remain
	if (!ActiveHighlightDescriptor && !ActivePromptDescriptor)
	{
		CachedIndicatorManager.Reset();
	}
}

void AAuraInteractableBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// CachedIndicatorManager ensures these remove calls succeed even with nullptr interactor
	ClearIndicator(nullptr, ActiveHighlightDescriptor);
	ClearIndicator(nullptr, ActivePromptDescriptor);

	Super::EndPlay(EndPlayReason);
}