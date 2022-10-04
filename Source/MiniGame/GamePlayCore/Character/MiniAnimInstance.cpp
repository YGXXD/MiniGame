// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniAnimInstance.h"

#include "MiniCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMiniAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PlayerOwner = Cast<AMiniCharacter>(TryGetPawnOwner());
}

void UMiniAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(PlayerOwner)
	{
		Speed = PlayerOwner->GetVelocity().Length();
		bIsInAir = PlayerOwner->GetCharacterMovement()->IsFalling();
	}
}
