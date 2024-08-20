// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
//#pragma region Related to Enhanced Input
//
//UPROPERTY(EditAnywhere, Category = Input)
//class UInputMappingContext* IMC_Player;
//
//UPROPERTY(EditAnywhere, Category = Input)
//class UInputAction* IA_Move;
//
//UPROPERTY(EditAnywhere, Category = Input)
//class UInputAction* IA_Look;
//
//UPROPERTY(EditAnywhere, Category = Input)
//class UInputAction* IA_Jump;
//
//UPROPERTY(EditAnywhere, Category = Input)
//class UInputAction* IA_StartRun;
//
//UPROPERTY(EditAnywhere, Category = Input)
//class UInputAction* IA_StopRun;
};
