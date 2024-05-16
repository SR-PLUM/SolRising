// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_SolRising.h"

#include "Character/Solaris.h"
#include "Item/Gun.h"

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
	InputComponent->BindAction("Aiming", EInputEvent::IE_Pressed, this, &APC_SolRising::Aiming);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &APC_SolRising::Fire);
	InputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &APC_SolRising::Interaction);
	InputComponent->BindAction("ToggleCamera", EInputEvent::IE_Pressed, this, &APC_SolRising::ToggleCamera);
	InputComponent->BindAction("SelectMainGun", EInputEvent::IE_Pressed, this, &APC_SolRising::SelectMainGun);
	InputComponent->BindAction("SelectSubGun", EInputEvent::IE_Pressed, this, &APC_SolRising::SelectSubGun);
	InputComponent->BindAction("Kneel", EInputEvent::IE_Pressed, this, &APC_SolRising::Kneel);
	InputComponent->BindAction("Prone", EInputEvent::IE_Pressed, this, &APC_SolRising::Prone);
	InputComponent->BindAction("Inventory", EInputEvent::IE_Pressed, this, &APC_SolRising::Inventory);
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
	if (bShowMouseCursor)
		return;

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
	if (bShowMouseCursor)
		return;

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

void APC_SolRising::Aiming()
{
	if (bShowMouseCursor)
		return;

	if (Solaris == nullptr)
	{
		auto TryGetSolaris = Cast<ASolaris>(GetPawn());
		if (TryGetSolaris == nullptr)
		{
			return;
		}

		Solaris = TryGetSolaris;
	}

	Solaris->Aiming();
}

void APC_SolRising::Fire()
{
	if (bShowMouseCursor)
		return;

	if (Solaris == nullptr)
	{
		auto TryGetSolaris = Cast<ASolaris>(GetPawn());
		if (TryGetSolaris == nullptr)
		{
			return;
		}

		Solaris = TryGetSolaris;
	}

	AGun* mainGun = Solaris->GetMainGun();
	if (mainGun)
	{
		mainGun->Fire();
	}
}

void APC_SolRising::Interaction()
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

	Solaris->Interaction();
}

void APC_SolRising::ToggleCamera()
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

	Solaris->TogglePerspective();
}

void APC_SolRising::Kneel()
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

	Solaris->Kneel();
}

void APC_SolRising::Prone()
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

	Solaris->Prone();
}

void APC_SolRising::Inventory()
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

	bool IsOpenedInventory = Solaris->Inventory();
	if (IsOpenedInventory)
	{
		int32 X, Y;
		GetViewportSize(X, Y);
		SetMouseLocation(X / 2, Y / 2);
		bShowMouseCursor = true;
	}
	else
	{
		FInputModeGameOnly GameMode;
		SetInputMode(GameMode);

		bShowMouseCursor = false;
	}
}

void APC_SolRising::SelectMainGun()
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

	Solaris->CurrentGun = Solaris->GetMainGun();

	Solaris->AttachMainGun();
}

void APC_SolRising::SelectSubGun()
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

	Solaris->CurrentGun = Solaris->GetSubGun();

	Solaris->AttachSubGun();

}