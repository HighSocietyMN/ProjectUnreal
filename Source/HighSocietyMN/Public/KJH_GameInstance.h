// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "KJH_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class HIGHSOCIETYMN_API UKJH_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UKJH_GameInstance();
	// ������ ���۵� �� ȣ��Ǵ� �Լ� (�������̵�)
	virtual void OnStart() override;

	// ��� UI�� �����ϴ� UI Manager�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UKJH_UIManager* UIManager;

	// UI ���� �������Ʈ Ŭ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> UIFactory;

	// ��� ���� ����
	UPROPERTY(EditAnywhere)
	class USoundBase* BGM = nullptr;

};
