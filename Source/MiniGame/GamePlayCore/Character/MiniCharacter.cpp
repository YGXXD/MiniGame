// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MiniGame/GamePlayCore/Controller/ShareCamera.h"

// Sets default values
AMiniCharacter::AMiniCharacter(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = GetMesh();
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(TEXT("SkeletalMesh'/Game/ThirdPersonAnim/UE4_Mannequin/Mesh/SM_TPBody.SM_TPBody'"));
	if(SK_Mesh.Succeeded())
	{
		MeshComp->SetSkeletalMesh(SK_Mesh.Object);
	}
	
	MovementComp = GetCharacterMovement();
}

void AMiniCharacter::SetShareCamera(AShareCamera* Camera)
{
	ShareCamera = Camera;
}

AShareCamera* AMiniCharacter::GetShareCamera() const
{
	return ShareCamera.Get();
}

void AMiniCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMiniCharacter::MoveForward(float Value)
{
	if(ShareCamera.Get() && Value)
	{
		const FRotator DirectionRotation(0,ShareCamera->GetCameraComp()->GetComponentRotation().Yaw,0);
		const FVector Direction = FRotationMatrix(DirectionRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction,Value);
	}
}

void AMiniCharacter::MoveRight(float Value)
{
	if(ShareCamera.Get() && Value)
	{
		const FRotator DirectionRotation(0,ShareCamera->GetCameraComp()->GetComponentRotation().Yaw,0);
		const FVector Direction = FRotationMatrix(DirectionRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction,Value);
	}
}

void AMiniCharacter::Turn(float Value)
{
	if(ShareCamera.Get() && Value)
	{
		ShareCamera->AddCameraYawInput(Value);
	}
}

