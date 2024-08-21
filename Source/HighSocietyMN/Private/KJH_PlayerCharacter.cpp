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
#include "KJH_UIManager.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "NPCCharacter.h"
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

	// 사운드 관련 처리를 위한 초기값 설정 (FootStep Sound)
	FootStepInterval = WalkFootStepInterval; 	// 기본적으로 걷기 간격을 초기값으로 설정
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
				PlayerController->PlayerCameraManager->ViewPitchMin = -30.0f;
				PlayerController->PlayerCameraManager->ViewPitchMax = 30.0f;
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

	////////// 근처에 상호작용 가능한 사물이 있으면 UI를 띄우기 위해 오브젝트 탐색 및 UI 업데이트 구간 시작 -------------------------------------------------------------------------
	// 상호작용 가능 오브젝트 탐색 및 UI 업데이트
	float InteractBoxRange = 200.0f; // 상호작용 범위 설정
	FVector PlayerLocation = GetActorLocation(); // 플레이어의 현재 위치

	// 주변 반경 내 모든 액터 탐색
	TArray<FHitResult> HitResults;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(InteractBoxRange); // 구 형태의 충돌 영역

	// 1) AmmoBox와 GrenadeBox 탐색 구간을 하나로 통합함. Ammo Box Class에서 Enum Class로 구분할 수 있게 설정해놓음.
	HitResults.Empty();
	bool bIsCharacterNearby = GetWorld()->SweepMultiByProfile(HitResults, PlayerLocation, PlayerLocation, FQuat::Identity, TEXT("Buffet"), CollisionShape);

	if (bIsCharacterNearby)
	{
		for (const FHitResult& Hit : HitResults)
		{
			ANPCCharacter* OverlappingBoxActor = Cast<ANPCCharacter>(Hit.GetActor());
			if (OverlappingBoxActor)
			{
				// AmmoBox 일 경우,
				if (OverlappingBoxActor->CharacterType == ECharacterType::Buffet)
				{
					if (UIManager)
					{
						UIManager->UpdateInteractionUIForBuffet(true); // AmmoBox UI 표시
					}
					return;
				}
				// GrenadeBox 일 경우,
				else if (OverlappingBoxActor->CharacterType == ECharacterType::Musk)
				{
					if (UIManager)
					{
						UIManager->UpdateInteractionUIForMusk(true); // GrenadeBox UI 표시
					}
					return;
				}
			}
		}
	}

	// 박스가 범위 내에 없다면 UI 숨기기
	if (UIManager)
	{
			UIManager->UpdateInteractionUIForBuffet(false); 
			UIManager->UpdateInteractionUIForMusk(false); 
	}


	////////// SFX 관련 처리 --------------------------------------------------------------------------------------------------------------------------------------------------------
	// FootStepSound 사운드 처리
	// FootStepSound가 유효한지 확인
	if (FootStepSound && GetCharacterMovement()->Velocity.Size() > 0.0f)
	{
		// 타이머가 설정된 경우에만 발자국 소리 재생
		static float TimeSinceLastStep = 0.0f;
		TimeSinceLastStep += DeltaTime;

		if (TimeSinceLastStep >= FootStepInterval)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FootStepSound, GetActorLocation());
			TimeSinceLastStep = 0.0f;
		}
	}


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

	// 사운드 관련 처리 (FootStep Sound)
	FootStepInterval = RunFootStepInterval; // 달리기 중에는 더 짧은 간격으로 발자국 소리를 재생
}

// 달리기 종료
void AKJH_PlayerCharacter::OnMyActionStopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	// 사운드 관련 처리 (FootStep Sound)
	FootStepInterval = WalkFootStepInterval; // // 걷기 간격으로 발자국 소리를 재생
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

