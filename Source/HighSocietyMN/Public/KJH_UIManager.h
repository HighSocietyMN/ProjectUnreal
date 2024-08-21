// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KJH_UIManager.generated.h"

/**
 * 
 */
UCLASS()
class HIGHSOCIETYMN_API UKJH_UIManager : public UUserWidget
{
	GENERATED_BODY()
	

public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Recording;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* BuffetInteractionImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* MuskInteractionImage;

	// ��ȣ�ۿ� UI�� Progress Bar�� �����ϴ� �Լ� (��ȣ�ۿ� Ÿ�Կ� ����)
	UFUNCTION(BlueprintCallable)
	void UpdateRecordingUI(bool bIsVisible);
	UFUNCTION(BlueprintCallable)
	void UpdateInteractionUIForBuffet(bool bIsVisible);
	UFUNCTION(BlueprintCallable)
	void UpdateInteractionUIForMusk(bool bIsVisible);
};
