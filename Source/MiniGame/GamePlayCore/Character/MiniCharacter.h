// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MiniCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FOnInteractDelegate, class AMiniCharacter*);

class AShareCamera;
class UBoxComponent;

UENUM()
enum EMiniStatus
{
	Walk,
	Push,
	Climb
};

UCLASS()
class MINIGAME_API AMiniCharacter : public ACharacter
{
	GENERATED_BODY()
	
	friend class AMiniController;
public:
	AMiniCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void SetShareCamera(AShareCamera* Camera);
	AShareCamera* GetShareCamera() const;
	EMiniStatus GetCharacterStatus() const;
	void SetCharacterStatus(EMiniStatus Status);
protected:
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	virtual void Jump() override;
	virtual void StopJumping() override;
	void Turn(float Value);
	void InteractPressed();
	void InteractReleased();

	UFUNCTION()
	void OnBoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	EMiniStatus CharacterStatus;
	
	FOnInteractDelegate OnInteractPressed;
	FOnInteractDelegate OnInteractReleased;

	UPROPERTY()
	TObjectPtr<AActor> CurrInteractActor;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> MeshComp;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UCharacterMovementComponent> MovementComp;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> BoxComp;
	
	UPROPERTY()
	TObjectPtr<AShareCamera> ShareCamera;
};
