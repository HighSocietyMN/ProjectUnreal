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
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
//#include "AudioCaptureComponent.h"
//#include "AudioDevice.h"
//#include "Sound/SoundSubmix.h"

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
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->GravityScale = 1.0f;
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
	
	// Enhanced Input Mapping Context 바인딩 
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		UEnhancedInputLocalPlayerSubsystem* SubSys =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (SubSys)
		{
			SubSys->AddMappingContext(IMC_Player, 0);
		}

		// PlayerController가 있다면, 접근하여 카메라 최대 각도 조정
		if (PlayerController)
		{
			if (PlayerController->PlayerCameraManager)
			{
				PlayerController->PlayerCameraManager->ViewPitchMin = -70.0f;
				PlayerController->PlayerCameraManager->ViewPitchMax = 70.0f;
			}
		}
	}

	/*AudioCaptureComp = CreateDefaultSubobject<UAudioCaptureComponent>(TEXT("AudioCaptureComp"));
	AudioCaptureComp->SetupAttachment(RootComponent);*/
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

	// Move 바인딩
	EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AKJH_PlayerCharacter::OnMyActionMove);

	// Run 바인딩
	EnhancedInputComponent->BindAction(IA_StartRun, ETriggerEvent::Triggered, this, &AKJH_PlayerCharacter::OnMyActionStartRun);
	EnhancedInputComponent->BindAction(IA_StopRun, ETriggerEvent::Completed, this, &AKJH_PlayerCharacter::OnMyActionStopRun);

	// Look 바인딩
	EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AKJH_PlayerCharacter::OnMyActionLook);

	// Jump 바인딩
	EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AKJH_PlayerCharacter::OnMyActionJump);

	// Interaction 바인딩
	EnhancedInputComponent->BindAction(IA_Interaction, ETriggerEvent::Started, this, &AKJH_PlayerCharacter::OnMyActionInteraction);
	EnhancedInputComponent->BindAction(IA_Interaction, ETriggerEvent::Completed, this, &AKJH_PlayerCharacter::OnMyActionInteractionEnd);
}


// 기본 움직임
void AKJH_PlayerCharacter::OnMyActionMove(const FInputActionValue& Value)
{

	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);

}

// 카메라 회전
void AKJH_PlayerCharacter::OnMyActionLook(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerPitchInput(0.5f * LookAxisVector.Y);
	AddControllerYawInput(0.5f * LookAxisVector.X);
}

// 점프 
void AKJH_PlayerCharacter::OnMyActionJump(const FInputActionValue& Value)
{
	Jump();
}

// 달리기 시작
void AKJH_PlayerCharacter::OnMyActionStartRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

// 달리기 종료
void AKJH_PlayerCharacter::OnMyActionStopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void AKJH_PlayerCharacter::OnMyActionInteraction(const FInputActionValue& Value)
{
	//AudioCaptureComp->Start();

	//USoundSubmix* Submix = NewObject<USoundSubmix>();
	//AudioCaptureComp->SoundSubmix = Submix;

	//// 상호작용 1) 일반 NPC 탐색 (SweepMultiByProfile을 사용하여 블록 처리 탐색)
	//if (bIsInteractCrowdCharacter && bIsInteractElonMusk && bIsInteractWarrenBuffett) // 이미 다른 상호작용 중이 아니라면
	//{
	//	float InteractBoxRange_CrowdCharacter = 50.0f; // 상호작용 범위를 50으로 설정

	//	TArray<FHitResult> HitResults_CrowdCharacter;
	//	FCollisionShape CollisionShape_CrowdCharacter;
	//	CollisionShape_CrowdCharacter.SetSphere(InteractBoxRange_CrowdCharacter);

	//	FVector PlayerLocation_CrowdCharacter = GetActorLocation();

	//	if (GetWorld()->SweepMultiByProfile(HitResults_CrowdCharacter, PlayerLocation_CrowdCharacter, PlayerLocation_CrowdCharacter, FQuat::Identity, TEXT("Crowd"), CollisionShape_CrowdCharacter))
	//	{
	//		for (const FHitResult& Hit : HitResults_CrowdCharacter)
	//		{
	//			/* 내가 만들 클래스 추가하는 부분 ex) AKJH_CrowdCharacter* OverlappingCrowdCharacter = Cast<AKJH_CrowdCharacter>(Hit.GetActor()); */
	//			if (OverlappingCrowdCharacter)
	//			{

	//				// 상호작용 시작 시 캐릭터의 방향 고정
	//				bUseControllerRotationYaw = false;

	//				bIsInteractCrowdCharacter = true; // 터렛 상호작용 중 상태를 true로 설정


	//				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Crowd Interaction started"));
	//				return;


	//			}
	//		}
	//		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("No Crowd nearby to Interact"));
	//	}
	//}
}

void AKJH_PlayerCharacter::OnMyActionInteractionEnd()
{
	//AudioCaptureComp->Stop();


}

