// Fill out your copyright notice in the Description page of Project Settings.


#include "WoodenBox.h"

#include "TimerManager.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MiniGame/GamePlayCore/Character/MiniCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWoodenBox::AWoodenBox(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	
	WoodenBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WoodenBoxMesh"));
	RootComponent = WoodenBoxMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WoodenBoxMesh(TEXT("StaticMesh'/Game/GamePlayAsset/map1/object/10_5map1_pushblock.10_5map1_pushblock'"));
	if(SM_WoodenBoxMesh.Succeeded())
	{
		WoodenBoxMesh->SetStaticMesh(SM_WoodenBoxMesh.Object);
	}
	WoodenBoxMesh->SetLinearDamping(0.01f);
	WoodenBoxMesh->SetMassOverrideInKg(NAME_None,200,true);
	//WoodenBoxMesh->SetConstraintMode(EDOFMode::Default);
	WoodenBoxMesh->SetMobility(EComponentMobility::Movable);
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionProfileName(TEXT("InteractedObj"));
	BoxComp->SetBoxExtent(FVector(110,110,110));
	BoxComp->SetRelativeLocation(FVector(0,0,100));
	
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(RootComponent);

	ForwardPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ForwardPoint"));
	ForwardPoint->SetupAttachment(RootComponent);
	ForwardPoint->SetRelativeLocation(FVector(145,0,0));

	BackPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BackPoint"));
	BackPoint->SetupAttachment(RootComponent);
	BackPoint->SetRelativeLocation(FVector(-145,0,0));
	
	RightPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RightPoint"));
	RightPoint->SetupAttachment(RootComponent);
	RightPoint->SetRelativeLocation(FVector(0,145,0));
	
	LeftPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LeftPoint"));
	LeftPoint->SetupAttachment(RootComponent);
	LeftPoint->SetRelativeLocation(FVector(0,-145,0));
}

void AWoodenBox::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void AWoodenBox::OnInteractExecute(AMiniCharacter* Character)
{
	UKismetSystemLibrary::PrintString(GetWorld(),"OnInteractExecute");
	
	//????????????????????????
	FVector BoxToCharacter = Character->GetActorLocation() - WoodenBoxMesh->GetComponentLocation();
	BoxToCharacter.Normalize();
	
	const FVector BoxForward = FRotationMatrix(ArrowComp->GetComponentRotation()).GetUnitAxis(EAxis::X);
	const FVector BoxBack = -BoxForward;
	const FVector BoxRight = FRotationMatrix(ArrowComp->GetComponentRotation()).GetUnitAxis(EAxis::Y);
	const FVector BoxLeft = -BoxRight;
	
	const float CosTheta1 = BoxToCharacter.Dot(BoxForward);
	float Temp = CosTheta1;
	FRotator PushRotator = UKismetMathLibrary::FindLookAtRotation(FVector::Zero(),-BoxForward);
	const USceneComponent* Point = ForwardPoint.Get();
	
	const float CosTheta2 = BoxToCharacter.Dot(BoxBack);
	if(Temp < CosTheta2)
	{
		Temp = CosTheta2;
		PushRotator = UKismetMathLibrary::FindLookAtRotation(FVector::Zero(),-BoxBack);
		Point = BackPoint.Get();
	}
	
	const float CosTheta3 = BoxToCharacter.Dot(BoxRight);
	if(Temp < CosTheta3)
	{
		Temp = CosTheta3;
		PushRotator = UKismetMathLibrary::FindLookAtRotation(FVector::Zero(),-BoxRight);
		Point = RightPoint.Get();
	}
	
	const float CosTheta4 = BoxToCharacter.Dot(BoxLeft);
	if(Temp < CosTheta4)
	{
		PushRotator = UKismetMathLibrary::FindLookAtRotation(FVector::Zero(),-BoxLeft);
		Point = LeftPoint.Get();
	}
	
	FVector StartLocation = Character->GetActorLocation();
	FRotator StartRotation = Character->GetActorRotation();
	
	FVector EndLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, StartLocation.Z);
	FRotator EndRotation = PushRotator;

	Character->SetCharacterStatus(EMiniStatus::Push);
	//????????????????????????
	Time = 0;
	auto StartPushLambda = [this,Character,StartLocation,StartRotation,EndLocation,EndRotation]()
	{
		Time += 0.02f;
		Character->SetActorRotation(FMath::Lerp<FRotator>(StartRotation, EndRotation, Time));
		Character->SetActorLocation(FMath::Lerp<FVector>(StartLocation, EndLocation, Time));
	
		if (Time >= 1.0f)
		{
			GetWorldTimerManager().PauseTimer(PushHandle);
			WoodenBoxMesh->SetSimulatePhysics(true);
			// auto PushBoxLambda = [this,Character]()
			// {
			// 	const FVector Target = Character->GetActorForwardVector() * 145 + Character->GetActorLocation();
			// 	const FVector NewBoxLocation = UKismetMathLibrary::VInterpTo(
			// 		this->GetActorLocation(), Target, GetWorld()->GetDeltaSeconds(), 100);
			// 	this->SetActorLocation(NewBoxLocation);
			// };
			//
			// const FTimerDelegate PushBoxDelegate = FTimerDelegate::CreateLambda(PushBoxLambda);
			// this->GetWorldTimerManager().SetTimer(this->PushHandle, PushBoxDelegate, 0.005f, true);
		}
	};
	const FTimerDelegate StartPushDelegate = FTimerDelegate::CreateLambda(StartPushLambda);
	GetWorldTimerManager().SetTimer(PushHandle,StartPushDelegate,0.01f,true);
	
}

void AWoodenBox::OnInteractExit(AMiniCharacter* Character)
{
	UKismetSystemLibrary::PrintString(GetWorld(),"OnInteractExit");
	if(GetWorldTimerManager().IsTimerActive(PushHandle))
		GetWorldTimerManager().PauseTimer(PushHandle);
	WoodenBoxMesh->SetSimulatePhysics(false);
	Character->SetCharacterStatus(EMiniStatus::Walk);
}

void AWoodenBox::OnInteractEnable(AMiniCharacter* Character)
{
	UKismetSystemLibrary::PrintString(GetWorld(),"OnInteractEnable");
}

void AWoodenBox::OnInteractDisable(AMiniCharacter* Character)
{
	UKismetSystemLibrary::PrintString(GetWorld(),"OnInteractDisable");
}

