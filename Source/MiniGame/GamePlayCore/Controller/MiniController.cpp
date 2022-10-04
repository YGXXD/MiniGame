// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniController.h"
#include "ShareCamera.h"
#include "Kismet/GameplayStatics.h"
#include "MiniGame/GamePlayCore/Character/MiniCharacter.h"

AMiniController::AMiniController(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMiniController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(AMiniCharacter* MiniCharacter = Cast<AMiniCharacter>(InPawn))
	{
		if(AShareCamera* ShareCamera = MiniCharacter->GetShareCamera())
		{
			FViewTargetTransitionParams TransitionParams;
			TransitionParams.BlendTime = 0;
			SetViewTarget(ShareCamera,TransitionParams);
		}
		switch (const int32 ControllerID = UGameplayStatics::GetPlayerControllerID(this))
		{
		case 0:
			{
				InputComponent->BindAxis(TEXT("MoveForwardOne"),MiniCharacter,&AMiniCharacter::MoveForward);
				InputComponent->BindAxis(TEXT("MoveRightOne"),MiniCharacter,&AMiniCharacter::MoveRight);
				InputComponent->BindAxis(TEXT("TurnOne"),MiniCharacter,&AMiniCharacter::Turn);
				InputComponent->BindAction(TEXT("JumpOne"),IE_Pressed,MiniCharacter,&AMiniCharacter::Jump);
				InputComponent->BindAction(TEXT("JumpOne"),IE_Released,MiniCharacter,&AMiniCharacter::StopJumping);
			}
			break;
		case 1:
			{
				UKismetSystemLibrary::PrintString(GetWorld(),TEXT("绑定成功"));
				InputComponent->BindAxis(TEXT("MoveForwardTwo"),MiniCharacter,&AMiniCharacter::MoveForward);
				InputComponent->BindAxis(TEXT("MoveRightTwo"),MiniCharacter,&AMiniCharacter::MoveRight);
				InputComponent->BindAction(TEXT("JumpTwo"),IE_Pressed,MiniCharacter,&AMiniCharacter::Jump);
				InputComponent->BindAction(TEXT("JumpTwo"),IE_Released,MiniCharacter,&AMiniCharacter::StopJumping);
			}
			break;
		default:
			break;
		}
	}
}

