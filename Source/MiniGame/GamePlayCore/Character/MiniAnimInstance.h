// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MiniAnimInstance.generated.h"

class AMiniCharacter;
UCLASS()
class MINIGAME_API UMiniAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY()
	AMiniCharacter* PlayerOwner;
	
	UPROPERTY(BlueprintReadOnly)
	float Speed;

	UPROPERTY(BlueprintReadOnly)
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly)
	bool bIsPush;

	UPROPERTY(BlueprintReadOnly)
	bool bIsClimb;
};
