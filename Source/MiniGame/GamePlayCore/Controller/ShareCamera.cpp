// Fill out your copyright notice in the Description page of Project Settings.


#include "ShareCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AShareCamera::AShareCamera(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorEnableCollision(false);
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 1200;
	SpringArmComp->SetRelativeRotation(FRotator(330,0,0));
	SpringArmComp->bDoCollisionTest = false;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

void AShareCamera::AddCameraYawInput(float Value) const
{
	FRotator Rotator = SpringArmComp->GetRelativeRotation();
	Rotator.Yaw = Rotator.Yaw + Value * CameraRotatorSpeed;
	SpringArmComp->SetRelativeRotation(Rotator);
}

UCameraComponent* AShareCamera::GetCameraComp() const
{
	return CameraComp.Get();
}
