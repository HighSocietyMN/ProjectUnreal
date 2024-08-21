// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH_GameInstance.h"
#include "KJH_UIManager.h"
#include "Kismet/GameplayStatics.h"

UKJH_GameInstance::UKJH_GameInstance()
{

}

void UKJH_GameInstance::OnStart()
{

	// UIFactory�� �����Ǿ� �ִ��� Ȯ��
	if (UIFactory)
	{
		// UIFactory�� ���� UIManager ���� ����
		UIManager = CreateWidget<UKJH_UIManager>(this, UIFactory);

		// ������ UIManager�� ��ȿ���� Ȯ��
		if (UIManager)
		{
			// UIManager�� ȭ�鿡 �߰�
			UIManager->AddToViewport();
		}
	}

	// BGM�� �����Ǿ� �ִ��� Ȯ��
	if (BGM)
	{
		// ��� ���� ���
		UGameplayStatics::PlaySound2D(this, BGM);
	}
}
