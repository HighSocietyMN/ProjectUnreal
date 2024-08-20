// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

// Sets default values
AKJH_PlayerCharacter::AKJH_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// 기본 템플릿 CapsuleComponent 설정 -------------------------------------------------------------------------------------------------------
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	// 기본 템플릿 ACharacter 메쉬 설정 --------------------------------------------------------------------------------------------------------
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	GetMesh()->SetCollisionProfileName(TEXT("Player"));

	// 기본 템플릿 ACharacterMovement 설정 -----------------------------------------------------------------------------------------------------
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->GravityScale = 3.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Spring 컴포넌트 설정 --------------------------------------------------------------------------------------------------------------------
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraRotationLag = true; // 카메라가 살짝 딜레이 걸리게 쫒아가게함.
	SpringArmComp->CameraRotationLagSpeed = 8.0f;

	// TPS Camera 컴포넌트 생성 ----------------------------------------------------------------------------------------------------------------
	TPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPSCamera"));
	TPSCamera->SetupAttachment(SpringArmComp);
	TPSCamera->SetRelativeLocation(FVector(125, 50, 70));
	TPSCamera->SetFieldOfView(90.0f);
}

// Called when the game starts or when spawned
void AKJH_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKJH_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKJH_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//// Move 바인딩
	//EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AKJH_PlayerCharacter::OnMyActionMove);

	//// Run 바인딩
	//EnhancedInputComponent->BindAction(IA_StartRun, ETriggerEvent::Triggered, this, &AKJH_PlayerCharacter::OnMyActionStartRun);
	//EnhancedInputComponent->BindAction(IA_StopRun, ETriggerEvent::Completed, this, &AKJH_PlayerCharacter::OnMyActionStopRun);

	//// Look 바인딩
	//EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AKJH_PlayerCharacter::OnMyActionLook);

	//// Jump 바인딩
	//EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AKJH_PlayerCharacter::OnMyActionJump);

}

