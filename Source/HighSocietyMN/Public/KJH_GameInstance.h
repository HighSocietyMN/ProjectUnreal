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
	// 게임이 시작될 때 호출되는 함수 (오버라이드)
	virtual void OnStart() override;

	// 모든 UI를 관리하는 UI Manager를 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UKJH_UIManager* UIManager;

	// UI 위젯 블루프린트 클래스 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> UIFactory;

	// 배경 음악 참조
	UPROPERTY(EditAnywhere)
	class USoundBase* BGM = nullptr;

};
