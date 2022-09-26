// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MiniCharacter.generated.h"


class AShareCamera;
UCLASS()
class MINIGAME_API AMiniCharacter : public ACharacter
{
	GENERATED_BODY()

	friend class AMiniController;
public:
	AMiniCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void SetShareCamera(AShareCamera* Camera);
	AShareCamera* GetShareCamera() const;
protected:
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
private:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> MeshComp;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UCharacterMovementComponent> MovementComp;
	
	UPROPERTY()
	TObjectPtr<AShareCamera> ShareCamera;
};
