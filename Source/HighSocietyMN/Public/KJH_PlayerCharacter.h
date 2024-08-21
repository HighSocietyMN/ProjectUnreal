// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "KJH_PlayerCharacter.generated.h"


UCLASS()
class HIGHSOCIETYMN_API AKJH_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKJH_PlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Spring Arm ������Ʈ ����
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	// 3��Ī ī�޶� ������Ʈ ����
	UPROPERTY(EditAnywhere)
	class UCameraComponent* TPSCamera;


	//////////// Enhanced Input ���� ��ǲ�� ���� ------------------------------------------------------------------------------------------------------
	#pragma region Related to Enhanced Input

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_StartRun;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_StopRun;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Interaction;

	////////// Enhanced Input ���� ��ǲ�� ���� ------------------------------------------------------------------------------------------------------

	////////// Enhanced Input ���� �Լ� ���� --------------------------------------------------------------------------------------------------------
	void OnMyActionMove(const FInputActionValue& Value); // �⺻ �����¿� ������
	void OnMyActionLook(const FInputActionValue& Value); // ī�޶� ȸ��
	void OnMyActionJump(const FInputActionValue& Value); // ����

	void OnMyActionStartRun(); // �޸��� ����
	void OnMyActionStopRun(); // �޸��� ����

	void OnMyActionInteraction(const FInputActionValue& Value); // ���ͷ���
	void OnMyActionInteractionEnd(); // ���ͷ��� ����

 
	// ����� ĸ��
	UPROPERTY(EditAnywhere)
	class UAudioCaptureComponent* AudioCaptureComp;


	UPROPERTY(EditAnywhere)
	// ���� �ν��Ͻ� ����
	class UKJH_UIManager* UIManager; // UI�Ŵ����� �������� ���� Ŭ����


UPROPERTY(EditAnywhere)
class USoundWave* FootStepSound; // ���ڱ� �Ҹ�

UPROPERTY(EditAnywhere)
float WalkFootStepInterval = 0.65f; // �ȱ� �� ���ڱ� �Ҹ��� ����Ǵ� ����

UPROPERTY(EditAnywhere)
float RunFootStepInterval = 0.40f; // �޸��� �� ���ڱ� �Ҹ��� ����Ǵ� ����

UPROPERTY(EditAnywhere)
float FootStepInterval; // ���� ���ڱ� �Ҹ� ��� ����
};
