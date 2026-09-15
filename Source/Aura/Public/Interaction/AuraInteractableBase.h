#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "UI/IndicatorSystem/IndicatorDescriptor.h"
#include "AuraInteractableBase.generated.h"

class UAuraInteractableComponent;

UCLASS(Abstract, Blueprintable)
class AURA_API AAuraInteractableBase : public AActor
{
	GENERATED_BODY()

public:
	AAuraInteractableBase();

	UFUNCTION()
	virtual void OnFocusGained(AActor* Interactor);

	UFUNCTION()
	virtual void OnFocusLost(AActor* Interactor);

	UFUNCTION()
	virtual void OnHighlightGained(AActor* Interactor);

	UFUNCTION()
	virtual void OnHighlightLost(AActor* Interactor);

	UFUNCTION()
	virtual void OnInteract(AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction|UI")
	USceneComponent* GetIndicatorTargetComponent() const;
	virtual USceneComponent* GetIndicatorTargetComponent_Implementation() const;

	FORCEINLINE UAuraInteractableComponent* GetInteractableComponent() const { return InteractableComponent; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UAuraInteractableComponent> InteractableComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Indicators")
	TSubclassOf<UUserWidget> HighlightIndicatorWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Indicators")
	TSubclassOf<UUserWidget> PromptIndicatorWidgetClass;

	void CreateIndicator(AActor* Interactor, TSubclassOf<UUserWidget> WidgetClass, TObjectPtr<UIndicatorDescriptor>& OutDescriptor);
	void ClearIndicator(AActor* Interactor, TObjectPtr<UIndicatorDescriptor>& Descriptor);

private:
	UPROPERTY()
	TObjectPtr<UIndicatorDescriptor> ActiveHighlightDescriptor;

	UPROPERTY()
	TObjectPtr<UIndicatorDescriptor> ActivePromptDescriptor;

	UPROPERTY()
	TWeakObjectPtr<UAuraIndicatorManagerComponent> CachedIndicatorManager;

	UAuraIndicatorManagerComponent* GetIndicatorManagerFromInteractor(AActor* Interactor) const;
};