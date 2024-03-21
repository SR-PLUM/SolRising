// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_SolRising.h"

#include "Character/Solaris.h"

APC_SolRising::APC_SolRising()
{
	bShowMouseCursor = false;
}

void APC_SolRising::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &APC_SolRising::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APC_SolRising::MoveRight);
	InputComponent->BindAxis("Turn", this, &APC_SolRising::Turn);
	InputComponent->BindAxis("LookUp", this, &APC_SolRising::LookUp);

	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &APC_SolRising::Jump);
}

void APC_SolRising::MoveForward(float Value)
{
	if (Solaris == nullptr)
	{
		auto TryGetSolaris = Cast<ASolaris>(GetPawn());
		if (TryGetSolaris == nullptr)
		{
			return;
		}

		Solaris = TryGetSolaris;
	}

	Solaris->MoveForward(Value);
}

void APC_SolRising::MoveRight(float Value)
{
	if (Solaris == nullptr)
	{
		auto TryGetSolaris = Cast<ASolaris>(GetPawn());
		if (TryGetSolaris == nullptr)
		{
			return;
		}

		Solaris = TryGetSolaris;
	}

	Solaris->MoveRight(Value);
}

void APC_SolRising::Turn(float Value)
{
	if (Solaris == nullptr)
	{
		auto TryGetSolaris = Cast<ASolaris>(GetPawn());
		if (TryGetSolaris == nullptr)
		{
			return;
		}

		Solaris = TryGetSolaris;
	}

	Solaris->Turn(Value);
}

void APC_SolRising::LookUp(float Value)
{
	if (Solaris == nullptr)
	{
		auto TryGetSolaris = Cast<ASolaris>(GetPawn());
		if (TryGetSolaris == nullptr)
		{
			return;
		}

		Solaris = TryGetSolaris;
	}
	
	Solaris->LookUp(Value);
}

void APC_SolRising::Jump()
{
	if (Solaris == nullptr)
	{
		auto TryGetSolaris = Cast<ASolaris>(GetPawn());
		if (TryGetSolaris == nullptr)
		{
			return;
		}

		Solaris = TryGetSolaris;
	}

	Solaris->Jump();
}
