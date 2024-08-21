// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/TimelineComponent.h"

#include "MessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class HIGHSOCIETYMN_API UMessageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	class UOverlay* NarrationOverlay;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UNarrationWidget> NarrationWidgetFactory;
	
	void AddNarrationWidget(UNarrationWidget* widget);

	UPROPERTY()
	FTimeline WidgetMoveTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCurveFloat* MoveCurve;

	UFUNCTION()
	void HandleProgress(float Value);

	UFUNCTION()
	void WidgetMoveEnd();
	
	UPROPERTY()
	TArray<FTimerHandle> DialogHnds;
	UPROPERTY(EditAnywhere, Category = "MyStruct")
    USoundBase* Sound;

	UFUNCTION()
	void ShowDialogForDuration(UTexture2D* Icon, FString Name, FString Description);

	UFUNCTION()
	void EnVisibleTextBlock(UNarrationWidget* widget);
};
