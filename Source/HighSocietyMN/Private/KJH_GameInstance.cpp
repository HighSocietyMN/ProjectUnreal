// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH_GameInstance.h"
#include "KJH_UIManager.h"
#include "Kismet/GameplayStatics.h"

UKJH_GameInstance::UKJH_GameInstance()
{

}

void UKJH_GameInstance::OnStart()
{

	// UIFactory가 설정되어 있는지 확인
	if (UIFactory)
	{
		// UIFactory를 통해 UIManager 위젯 생성
		UIManager = CreateWidget<UKJH_UIManager>(this, UIFactory);

		// 생성된 UIManager가 유효한지 확인
		if (UIManager)
		{
			// UIManager를 화면에 추가
			UIManager->AddToViewport();
		}
	}

	// BGM이 설정되어 있는지 확인
	if (BGM)
	{
		// 배경 음악 재생
		UGameplayStatics::PlaySound2D(this, BGM);
	}
}
