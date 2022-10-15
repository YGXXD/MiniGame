// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniCharacter.h"

#include "MiniAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MiniGame/GamePlayCore/Controller/ShareCamera.h"
#include "MiniGame/GamePlayCore/Interaction/InteractionInterface.h"
#include "UObject/ConstructorHelpers.h"

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
	MovementComp->JumpZVelocity = 700.f;
	MovementComp->AirControl = 0.8f;
	MovementComp->GravityScale = 2.f;
	MovementComp->BrakingDecelerationFlying = 100.f;
	SetCharacterStatus(EMiniStatus::Walk);

	CurrInteractActor = nullptr;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetRelativeLocation(FVector(40,0,0));
	BoxComp->SetBoxExtent(FVector(40,16,16));
	BoxComp->SetCollisionProfileName(TEXT("InteractAll"));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&AMiniCharacter::OnBoxCompBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AMiniCharacter::OnBoxCompEndOverlap);
	
}

void AMiniCharacter::SetShareCamera(AShareCamera* Camera)
{
	ShareCamera = Camera;
}

AShareCamera* AMiniCharacter::GetShareCamera() const
{
	return ShareCamera.Get();
}

EMiniStatus AMiniCharacter::GetCharacterStatus() const
{
	return CharacterStatus;
}

void AMiniCharacter::SetCharacterStatus(EMiniStatus Status)
{
	CharacterStatus = Status;
	switch (CharacterStatus)
	{
	case EMiniStatus::Walk:
		{
			MovementComp->bOrientRotationToMovement = true;
			MovementComp->SetMovementMode(EMovementMode::MOVE_Walking);
			MovementComp->MaxWalkSpeed = 300.f;
		}
		break;
	case EMiniStatus::Push:
		{
			MovementComp->bOrientRotationToMovement = true;
			MovementComp->SetMovementMode(EMovementMode::MOVE_Walking);
			MovementComp->MaxWalkSpeed = 150.f;
		}
		break;
	case EMiniStatus::Climb:
		{
			MovementComp->bOrientRotationToMovement = false;
			MovementComp->MaxFlySpeed = 100.f;
			MovementComp->SetMovementMode(EMovementMode::MOVE_Flying);
		}
	default:
		break;
	}
}

void AMiniCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterStatus(EMiniStatus::Walk);
}

void AMiniCharacter::MoveForward(float Value)
{
	if(ShareCamera.Get() && Value)
	{
		switch (CharacterStatus)
		{
		case EMiniStatus::Walk:
			{
				const FRotator DirectionRotation(0,ShareCamera->GetCameraComp()->GetComponentRotation().Yaw,0);
				const FVector Direction = FRotationMatrix(DirectionRotation).GetUnitAxis(EAxis::X);
				AddMovementInput(Direction,Value);
			}
			break;
		case EMiniStatus::Push:
			{
				const FRotator DirectionRotation(0,ShareCamera->GetCameraComp()->GetComponentRotation().Yaw,0);
				const FVector Direction = FRotationMatrix(DirectionRotation).GetUnitAxis(EAxis::X);
				if(Value > 0 && GetActorForwardVector().Dot(Direction) >= 0)
				{
					AddMovementInput(GetActorForwardVector(),Value);
				}
				else if(Value < 0 && GetActorForwardVector().Dot(Direction) < 0)
				{
					AddMovementInput(GetActorForwardVector(),-Value);
				}
			}
			break;
		case EMiniStatus::Climb:
			{
				AddMovementInput(GetActorUpVector(),Value);
			}
			break;
		default:
			break;
		}
		
	}
}

void AMiniCharacter::MoveRight(float Value)
{
	if(ShareCamera.Get() && Value)
	{
		switch (CharacterStatus)
		{
		case EMiniStatus::Walk:
			{
				const FRotator DirectionRotation(0,ShareCamera->GetCameraComp()->GetComponentRotation().Yaw,0);
				const FVector Direction = FRotationMatrix(DirectionRotation).GetUnitAxis(EAxis::Y);
				AddMovementInput(Direction,Value);
			}
			break;
		case EMiniStatus::Push:
			{
				const FRotator DirectionRotation(0,ShareCamera->GetCameraComp()->GetComponentRotation().Yaw,0);
				const FVector Direction = FRotationMatrix(DirectionRotation).GetUnitAxis(EAxis::Y);
				if(Value > 0 && GetActorForwardVector().Dot(Direction) >= 0)
				{
					AddMovementInput(GetActorForwardVector(),Value);
				}
				else if(Value < 0 && GetActorForwardVector().Dot(Direction) < 0)
				{
					AddMovementInput(GetActorForwardVector(),-Value);
				}
			}
			break;
		case EMiniStatus::Climb:
			break;
		default:
			break;
		}
	}
}

void AMiniCharacter::Jump()
{
	if(CharacterStatus == Walk)
		Super::Jump();
}

void AMiniCharacter::StopJumping()
{
	Super::StopJumping();
}

void AMiniCharacter::Turn(float Value)
{
	if(ShareCamera.Get() && Value)
	{
		ShareCamera->AddCameraYawInput(Value);
	}
}

void AMiniCharacter::InteractPressed()
{
	OnInteractPressed.ExecuteIfBound(this);
}

void AMiniCharacter::InteractReleased()
{
	OnInteractReleased.ExecuteIfBound(this);
}

void AMiniCharacter::OnBoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInteractionInterface* Interface = Cast<IInteractionInterface>(OtherActor);
	if(Interface && !CurrInteractActor.Get())
	{
		CurrInteractActor = OtherActor;
		OnInteractPressed.BindRaw(Interface, &IInteractionInterface::OnInteractExecute);
		OnInteractReleased.BindRaw(Interface, &IInteractionInterface::OnInteractExit);
		Interface->OnInteractEnable(this);
	}
}

void AMiniCharacter::OnBoxCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInteractionInterface* Interface = Cast<IInteractionInterface>(OtherActor);
	if(Interface && CurrInteractActor.Get() == OtherActor)
	{
		CurrInteractActor = nullptr;
		OnInteractPressed.Unbind();
		OnInteractReleased.Unbind();
		Interface->OnInteractDisable(this);
	}
}
