// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SolarisAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Character/Solaris.h"

void USolarisAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Solaris = Cast<ASolaris>(TryGetPawnOwner());
	if (Solaris)
	{
		SolarisMovement = Solaris->GetCharacterMovement();
	}
}

void USolarisAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (SolarisMovement)
	{
		auto velocity = SolarisMovement->Velocity;

		GroundSpeed = UKismetMathLibrary::VSizeXY(velocity);

		auto rotation = Solaris->GetActorRotation();
		Direction = CalculateDirection(velocity, rotation);

		IsFalling = SolarisMovement->IsFalling();
	}

	if (Solaris)
	{
		if (Solaris->GetMainGun())
		{
			IsEquippedGun = true;
			Solaris->bUseControllerRotationYaw = true;
		}
		else
		{
			IsEquippedGun = false;
			Solaris->bUseControllerRotationYaw = false;
		}
	}
}
