// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShareCamera.generated.h"

class USpringArmComponent;
class UCameraComponent;
UCLASS()
class MINIGAME_API AShareCamera : public AActor
{
	GENERATED_BODY()

public:	
	AShareCamera(const FObjectInitializer& ObjectInitializer);

	void AddCameraYawInput(float Value) const;
	UCameraComponent* GetCameraComp() const;
	
private:
	int32 CameraRotatorSpeed = 1;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComp;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UCameraComponent> CameraComp;
};
