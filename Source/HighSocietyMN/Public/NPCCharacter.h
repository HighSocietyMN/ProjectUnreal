// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCCharacter.generated.h"


// 구분을 위해 EnumClass 로 AmmoBox와 GrenadeBox 구분
UENUM(BlueprintType)
enum class ECharacterType : uint8
{
    Buffet UMETA(DisplayName = "Buffet"),
    Musk UMETA(DisplayName = "Musk")
};


UCLASS()
class HIGHSOCIETYMN_API ANPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FString NpcName;


		// 구분을 위해 EnumClass 로 AmmoBox와 GrenadeBox 구분
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Type")
	ECharacterType CharacterType;

};
