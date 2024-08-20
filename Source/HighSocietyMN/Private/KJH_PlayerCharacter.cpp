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


	// �⺻ ���ø� CapsuleComponent ���� -------------------------------------------------------------------------------------------------------
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	// �⺻ ���ø� ACharacter �޽� ���� --------------------------------------------------------------------------------------------------------
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	GetMesh()->SetCollisionProfileName(TEXT("Player"));

	// �⺻ ���ø� ACharacterMovement ���� -----------------------------------------------------------------------------------------------------
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->GravityScale = 3.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Spring ������Ʈ ���� --------------------------------------------------------------------------------------------------------------------
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraRotationLag = true; // ī�޶� ��¦ ������ �ɸ��� �i�ư�����.
	SpringArmComp->CameraRotationLagSpeed = 8.0f;

	// TPS Camera ������Ʈ ���� ----------------------------------------------------------------------------------------------------------------
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

	//// Move ���ε�
	//EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AKJH_PlayerCharacter::OnMyActionMove);

	//// Run ���ε�
	//EnhancedInputComponent->BindAction(IA_StartRun, ETriggerEvent::Triggered, this, &AKJH_PlayerCharacter::OnMyActionStartRun);
	//EnhancedInputComponent->BindAction(IA_StopRun, ETriggerEvent::Completed, this, &AKJH_PlayerCharacter::OnMyActionStopRun);

	//// Look ���ε�
	//EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AKJH_PlayerCharacter::OnMyActionLook);

	//// Jump ���ε�
	//EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AKJH_PlayerCharacter::OnMyActionJump);

}

