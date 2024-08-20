// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH_PlayerAnimInstance.h"

void UKJH_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{

    // PlayerCharacter의 참조를 가져옴
    AKJH_PlayerCharacter* PlayerCharacter = Cast<AKJH_PlayerCharacter>(TryGetPawnOwner());
    if (nullptr != PlayerCharacter)
    {
        FVector velocity = PlayerCharacter->GetVelocity();
        FVector forwardVector = PlayerCharacter->GetActorForwardVector();
        FVector rightVector = PlayerCharacter->GetActorRightVector();

        // 앞 뒤 속도
        Speed = FVector::DotProduct(forwardVector, velocity);

        // 좌우 속도
        Direction = FVector::DotProduct(rightVector, velocity);
    }
}
