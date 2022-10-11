// Copyright Epic Games, Inc. All Rights Reserved.


#include "MiniGameGameModeBase.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "MiniGame/GamePlayCore/Character/MiniCharacter.h"
#include "MiniGame/GamePlayCore/Controller/MiniController.h"
#include "MiniGame/GamePlayCore/Controller/ShareCamera.h"

AMiniGameGameModeBase::AMiniGameGameModeBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = nullptr;
	PlayerControllerClass = AMiniController::StaticClass();
}

void AMiniGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::CreatePlayer(GetWorld());

	if(UWorld* ThisWord = GetWorld())
	{
		//生成共享相机
		AShareCamera* ShareCamera = nullptr;
		TActorIterator<AShareCamera> ShareCameraIt(ThisWord, AShareCamera::StaticClass());
		if(ShareCameraIt)
		{
			ShareCamera = *ShareCameraIt;
		}
		else
		{
			const FVector SpawnLocationCamera(0,0,0);
			const FRotator SpawnRotationCamera(0,0,0);
			FActorSpawnParameters ParametersCamera;
			ParametersCamera.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ShareCamera = ThisWord->SpawnActor<AShareCamera>(AShareCamera::StaticClass(),SpawnLocationCamera,SpawnRotationCamera,ParametersCamera);
		}

		//获取角色出生点
		FVector SpawnLocationOne(0,0,100);
		FRotator SpawnRotationOne(0,0,0);
		FVector SpawnLocationTwo(20,20,100);
		FRotator SpawnRotationTwo(0,0,0);
		for(TActorIterator<APlayerStart> PlayerStartIt(ThisWord, APlayerStart::StaticClass()); PlayerStartIt; ++PlayerStartIt)
		{
			if((*PlayerStartIt)->PlayerStartTag == TEXT("One"))
			{
				SpawnLocationOne = (*PlayerStartIt)->GetActorLocation();
				SpawnRotationOne = (*PlayerStartIt)->GetActorRotation();
			}
			else if((*PlayerStartIt)->PlayerStartTag == TEXT("Two"))
			{
				SpawnLocationTwo = (*PlayerStartIt)->GetActorLocation();
				SpawnRotationTwo = (*PlayerStartIt)->GetActorRotation();
			}
		}
		
		//生成角色1
		FActorSpawnParameters ParametersOne;
		ParametersOne.Owner = UGameplayStatics::GetPlayerController(ThisWord,0);
		ParametersOne.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AMiniCharacter* CharacterOne = ThisWord->SpawnActor<AMiniCharacter>(AMiniCharacter::StaticClass(),SpawnLocationOne,SpawnRotationOne,ParametersOne);
		CharacterOne->SetShareCamera(ShareCamera);
		UGameplayStatics::GetPlayerController(ThisWord,0)->Possess(CharacterOne);

		//生成角色2
		FActorSpawnParameters ParametersTwo;
		ParametersTwo.Owner = UGameplayStatics::GetPlayerController(ThisWord,1);
		ParametersTwo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AMiniCharacter* CharacterTwo = ThisWord->SpawnActor<AMiniCharacter>(AMiniCharacter::StaticClass(),SpawnLocationTwo,SpawnRotationTwo,ParametersTwo);
		CharacterTwo->SetShareCamera(ShareCamera);
		UGameplayStatics::GetPlayerController(ThisWord,1)->Possess(CharacterTwo);
	}
}

void AMiniGameGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}



