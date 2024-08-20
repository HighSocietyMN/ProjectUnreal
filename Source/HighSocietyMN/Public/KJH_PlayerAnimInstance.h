// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KJH_PlayerCharacter.h"
#include "KJH_PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HIGHSOCIETYMN_API UKJH_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	  // Vertical
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Speed;

	// Horizontal
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Direction;

};
