// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "Ladder.generated.h"

class AMiniCharacter;
class UBoxComponent;
class UArrowComponent;

UCLASS()
class MINIGAME_API ALadder : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ALadder(const FObjectInitializer& ObjectInitializer);

	virtual void OnInteractExecute(AMiniCharacter* Character) override;
	virtual void OnInteractExit(AMiniCharacter* Character) override;
	
	virtual void OnInteractEnable(AMiniCharacter* Character) override;
	virtual void OnInteractDisable(AMiniCharacter* Character) override;
protected:
	virtual void BeginPlay() override;

protected:
	bool bIsUsed;
	
	float Time;
	FTimerHandle ClimbHandle;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> LadderMesh;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UArrowComponent> ArrowComp;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> ForwardPoint;

};
