// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageWidget.h"
#include "NarrationWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TimelineComponent.h"

void UMessageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MoveCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleProgress"));

		// 종료
		FOnTimelineEvent FinishedEvent;
		FinishedEvent.BindUFunction(this, FName("WidgetMoveEnd"));

		WidgetMoveTimeline.AddInterpFloat(MoveCurve, ProgressFunction);
		WidgetMoveTimeline.SetTimelineFinishedFunc(FinishedEvent);

		WidgetMoveTimeline.SetLooping(false);
	}
}

void UMessageWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UE_LOG(LogTemp, Warning, TEXT("UHD_PlayerWidget::NativeDestruct()"));
	for (auto& DialogHnd : DialogHnds)
	{
		GetWorld()->GetTimerManager().ClearTimer(DialogHnd);
	}

	DialogHnds.Empty();
}

void UMessageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (WidgetMoveTimeline.IsPlaying())
	{
		WidgetMoveTimeline.TickTimeline(InDeltaTime);
	}
}

void UMessageWidget::AddNarrationWidget(UNarrationWidget* widget)
{
	// Overlay에 자식이 있다면 마지막 자식을 제거한다.
	if (NarrationOverlay->GetChildrenCount() > 0)
	{
		auto* LastWidget = Cast<UNarrationWidget>(NarrationOverlay->GetChildAt(NarrationOverlay->GetChildrenCount() - 1));
		if (LastWidget)
		{
			LastWidget->AnimNextWidget();
		}
	}

	NarrationOverlay->AddChild(widget);
	FTimerHandle DialogHnd;

	WidgetMoveTimeline.PlayFromStart();
	widget->AnimStartWidget();

	UE_LOG(LogTemp, Warning, TEXT("UHD_PlayerWidget::AddNarrationWidget()"));
	GetWorld()->GetTimerManager().SetTimer(DialogHnd, [this, &DialogHnd, widget]() {
		if (widget)
		{
			DialogHnds.Remove(DialogHnd);
			EnVisibleTextBlock(widget);
			widget->AnimHideWidget();
		}
		}, 15.0f, false);

	DialogHnds.Add(DialogHnd);
}

void UMessageWidget::HandleProgress(float Value)
{
	for (auto* Widget : NarrationOverlay->GetAllChildren())
	{
		// 마지막으로 들어온 애는 제외
		if (Widget == NarrationOverlay->GetChildAt(NarrationOverlay->GetChildrenCount() - 1))
		{
			continue;
		}
		auto* NarrationWidget = Cast<UNarrationWidget>(Widget);
		if (NarrationWidget)
		{
			// 기존 위치에서 value만큼 이동시킨다.
			NarrationWidget->SetRenderTranslation(FVector2D(0, NarrationWidget->yPos + Value));
		}
	}
}

void UMessageWidget::WidgetMoveEnd()
{
	for (auto* Widget : NarrationOverlay->GetAllChildren())
	{
		// 마지막으로 들어온 애는 제외
		if (Widget == NarrationOverlay->GetChildAt(NarrationOverlay->GetChildrenCount() - 1))
		{
			continue;
		}

		auto* NarrationWidget = Cast<UNarrationWidget>(Widget);
		if (NarrationWidget)
		{
			// 기존 위치에서 value만큼 이동시킨다.
			NarrationWidget->yPos += -100;
		}
	}
}

void UMessageWidget::ShowDialogForDuration(UTexture2D* Icon, FString Name, FString Description)
{
	auto* WidgetItem = Cast<UNarrationWidget>(CreateWidget(GetWorld(), NarrationWidgetFactory));

	WidgetItem->MessageStr = Description;
	WidgetItem->NarrationImage->SetBrushFromTexture(Icon);
	WidgetItem->NarrationText->SetText(FText::FromString(Name));

	AddNarrationWidget(WidgetItem);
}

void UMessageWidget::EnVisibleTextBlock(UNarrationWidget* widget)
{
	FTimerHandle DialogHnd;
	UE_LOG(LogTemp, Warning, TEXT("UHD_PlayerWidget::EnVisibleTextBlock()"));
	// 제거한다.
	GetWorld()->GetTimerManager().SetTimer(DialogHnd, [this, &DialogHnd, widget]() {
		UE_LOG(LogTemp, Warning, TEXT("UHD_PlayerWidget::EnVisibleTextBlock() SetTimer End"));
		if (NarrationOverlay)
			NarrationOverlay->RemoveChild(widget);
		DialogHnds.Remove(DialogHnd);
		}, 0.4f, false);

	DialogHnds.Add(DialogHnd);
}
