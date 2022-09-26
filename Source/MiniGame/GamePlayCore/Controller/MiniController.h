// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MiniController.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAME_API AMiniController : public APlayerController
{
	GENERATED_BODY()
public:

	AMiniController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void OnPossess(APawn* InPawn) override;
};
