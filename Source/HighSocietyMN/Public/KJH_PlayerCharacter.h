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


	// Spring Arm 컴포넌트 선언
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	// 3인칭 카메라 컴포넌트 선언
	UPROPERTY(EditAnywhere)
	class UCameraComponent* TPSCamera;


	//////////// Enhanced Input 관련 인풋값 시작 ------------------------------------------------------------------------------------------------------
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

	////////// Enhanced Input 관련 인풋값 종료 ------------------------------------------------------------------------------------------------------

	////////// Enhanced Input 관련 함수 시작 --------------------------------------------------------------------------------------------------------
	void OnMyActionMove(const FInputActionValue& Value); // 기본 상하좌우 움직임
	void OnMyActionLook(const FInputActionValue& Value); // 카메라 회전
	void OnMyActionJump(const FInputActionValue& Value); // 점프

	void OnMyActionStartRun(); // 달리기 시작
	void OnMyActionStopRun(); // 달리기 종료

	void OnMyActionInteraction(const FInputActionValue& Value); // 인터렉션
	void OnMyActionInteractionEnd(); // 인터렉션 종료

 
	// 오디오 캡쳐
	UPROPERTY(EditAnywhere)
	class UAudioCaptureComponent* AudioCaptureComp;


	UPROPERTY(EditAnywhere)
	// 게임 인스턴스 관련
	class UKJH_UIManager* UIManager; // UI매니저를 꺼내오기 위한 클래스


UPROPERTY(EditAnywhere)
class USoundWave* FootStepSound; // 발자국 소리

UPROPERTY(EditAnywhere)
float WalkFootStepInterval = 0.65f; // 걷기 시 발자국 소리가 재생되는 간격

UPROPERTY(EditAnywhere)
float RunFootStepInterval = 0.40f; // 달리기 시 발자국 소리가 재생되는 간격

UPROPERTY(EditAnywhere)
float FootStepInterval; // 현재 발자국 소리 재생 간격
};
