// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH_UIManager.h"
#include "Components/Image.h"

void UKJH_UIManager::NativeConstruct()
{
    BuffetInteractionImage->SetVisibility(ESlateVisibility::Hidden);
    MuskInteractionImage->SetVisibility(ESlateVisibility::Hidden);

}

void UKJH_UIManager::UpdateInteractionUIForBuffet(bool bIsVisible)
{
    if (BuffetInteractionImage)
    {
        if (bIsVisible)
        {
            BuffetInteractionImage ->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            BuffetInteractionImage ->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UKJH_UIManager::UpdateInteractionUIForMusk(bool bIsVisible)
{

    if (MuskInteractionImage)
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

}
