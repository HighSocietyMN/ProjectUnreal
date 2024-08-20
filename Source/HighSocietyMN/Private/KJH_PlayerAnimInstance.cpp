// Fill out your copyright notice in the Description page of Project Settings.


#include "KJH_PlayerAnimInstance.h"

void UKJH_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{

    // PlayerCharacter�� ������ ������
    AKJH_PlayerCharacter* PlayerCharacter = Cast<AKJH_PlayerCharacter>(TryGetPawnOwner());
    if (nullptr != PlayerCharacter)
    {
        FVector velocity = PlayerCharacter->GetVelocity();
        FVector forwardVector = PlayerCharacter->GetActorForwardVector();
        FVector rightVector = PlayerCharacter->GetActorRightVector();

        // �� �� �ӵ�
        Speed = FVector::DotProduct(forwardVector, velocity);

        // �¿� �ӵ�
        Direction = FVector::DotProduct(rightVector, velocity);
    }
}
