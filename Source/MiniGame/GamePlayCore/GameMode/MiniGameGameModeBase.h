// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MiniGameGameModeBase.generated.h"

UCLASS()
class MINIGAME_API AMiniGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMiniGameGameModeBase(const FObjectInitializer& ObjectInitializer);

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;


};
