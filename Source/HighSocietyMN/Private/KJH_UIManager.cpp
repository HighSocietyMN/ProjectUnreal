#include "KJH_UIManager.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH_UIManager.h"
#include "Components/Image.h"

void UKJH_UIManager::NativeConstruct()
{
    BuffetInteractionImage->SetVisibility(ESlateVisibility::Hidden);
    MuskInteractionImage->SetVisibility(ESlateVisibility::Hidden);
	Recording->SetVisibility(ESlateVisibility::Hidden);
}

void UKJH_UIManager::UpdateRecordingUI(bool bIsVisible)
{
	if (bIsVisible)
	{
		Recording->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Recording->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UKJH_UIManager::UpdateInteractionUIForBuffet(bool bIsVisible)
{
	if (bIsVisible)
	{
		BuffetInteractionImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BuffetInteractionImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UKJH_UIManager::UpdateInteractionUIForMusk(bool bIsVisible)
{
	if (bIsVisible)
	{
		MuskInteractionImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		MuskInteractionImage->SetVisibility(ESlateVisibility::Hidden);
	}
}
