// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "MiniGame/GamePlayCore/Character/MiniCharacter.h"

ALadder::ALadder(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	LadderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WoodenBoxMesh"));
	RootComponent = LadderMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WoodenBoxMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if(SM_WoodenBoxMesh.Succeeded())
	{
		LadderMesh->SetStaticMesh(SM_WoodenBoxMesh.Object);
	}
	LadderMesh->SetLinearDamping(0.01f);
	LadderMesh->SetMassOverrideInKg(NAME_None,200,true);
	LadderMesh->SetMobility(EComponentMobility::Movable);
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionProfileName(TEXT("InteractedObj"));
	//BoxComp->SetBoxExtent(FVector(110,110,110));
	//BoxComp->SetRelativeLocation(FVector(0,0,100));
	
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(RootComponent);
	ArrowComp->SetRelativeRotation(FRotator(0,180,0));

	ForwardPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ForwardPoint"));
	ForwardPoint->SetupAttachment(RootComponent);
	ForwardPoint->SetRelativeLocation(FVector(80,0,80));
}

void ALadder::OnInteractExecute(AMiniCharacter* Character)
{
	if(bIsUsed)
	{
		bIsUsed = false;
		Character->SetCharacterStatus(EMiniStatus::Walk);
	}
	else
	{
		bIsUsed = true;
		FVector StartLocation = Character->GetActorLocation();
		FRotator StartRotation = Character->GetActorRotation();
	
		FVector EndLocation = FVector(ForwardPoint->GetComponentLocation().X, ForwardPoint->GetComponentLocation().Y, StartLocation.Z);
		FRotator EndRotation = ArrowComp->GetComponentRotation();

		Character->SetCharacterStatus(EMiniStatus::Climb);
		//开始爬梯子的动作
		Time = 0;
		auto StartPushLambda = [this,Character,StartLocation,StartRotation,EndLocation,EndRotation]()
		{
			Time += 0.02f;
			Character->SetActorRotation(FMath::Lerp<FRotator>(StartRotation, EndRotation, Time));
			Character->SetActorLocation(FMath::Lerp<FVector>(StartLocation, EndLocation, Time));
	
			if (Time >= 1.0f)
			{
				GetWorldTimerManager().PauseTimer(ClimbHandle);
			}
		};
		const FTimerDelegate StartPushDelegate = FTimerDelegate::CreateLambda(StartPushLambda);
		GetWorldTimerManager().SetTimer(ClimbHandle,StartPushDelegate,0.01f,true);
	}
}

void ALadder::OnInteractExit(AMiniCharacter* Character)
{
}

void ALadder::OnInteractEnable(AMiniCharacter* Character)
{
	bIsUsed = false;
}

void ALadder::OnInteractDisable(AMiniCharacter* Character)
{
	bIsUsed = false;
	Character->SetCharacterStatus(EMiniStatus::Walk);
}

void ALadder::BeginPlay()
{
	Super::BeginPlay();
	
}
