// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "WoodenBox.generated.h"

class UArrowComponent;
class UBoxComponent;
class UCurveFloat;
UCLASS()
class MINIGAME_API AWoodenBox : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:	
	AWoodenBox(const FObjectInitializer& ObjectInitializer);
	
	virtual void OnInteractExecute(AMiniCharacter* Character) override;
	virtual void OnInteractExit(AMiniCharacter* Character) override;
	
	virtual void OnInteractEnable(AMiniCharacter* Character) override;
	virtual void OnInteractDisable(AMiniCharacter* Character) override;

protected:
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	float Time;
	FTimerHandle PushHandle;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComp;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> WoodenBoxMesh;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UArrowComponent> ArrowComp;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> ForwardPoint;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> BackPoint;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> LeftPoint;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> RightPoint;
};
