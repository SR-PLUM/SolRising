// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_SolRising.generated.h"

/**
 * 
 */
UCLASS()
class SOLRISING_API APC_SolRising : public APlayerController
{
	GENERATED_BODY()

public:
	APC_SolRising();
	
	void SetupInputComponent() override;

private:
	class ASolaris* Solaris;

public:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void Jump();

};
