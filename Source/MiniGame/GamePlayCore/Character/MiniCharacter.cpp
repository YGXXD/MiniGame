// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniCharacter.h"

#include "MiniAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MiniGame/GamePlayCore/Controller/ShareCamera.h"

// Sets default values
AMiniCharacter::AMiniCharacter(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	
	MeshComp = GetMesh();
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(TEXT("SkeletalMesh'/Game/GamePlayAsset/Character/mesh/MainChacrcter3.MainChacrcter3'"));
	if(SK_Mesh.Succeeded())
	{
		MeshComp->SetSkeletalMesh(SK_Mesh.Object);
	}
	static ConstructorHelpers::FClassFinder<UMiniAnimInstance> ABP_MiniAnim(TEXT("AnimBlueprint'/Game/GamePlayCore/Character/ABP_MiniCharacterAnim.ABP_MiniCharacterAnim_C'"));
	if(ABP_MiniAnim.Succeeded())
	{
		MeshComp->AnimClass = ABP_MiniAnim.Class;
	}
	MeshComp->SetRelativeLocation(FVector(0,0,-90));
	MeshComp->SetRelativeRotation(FRotator(0,-90,0));
	
	MovementComp = GetCharacterMovement();
	MovementComp->RotationRate = FRotator(0,720,0);
	MovementComp->bOrientRotationToMovement = true;
	MovementComp->JumpZVelocity = 600.f;
	MovementComp->AirControl = 0.8f;
	MovementComp->MaxWalkSpeed = 300.f;
	MovementComp->GravityScale = 2.f;
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

