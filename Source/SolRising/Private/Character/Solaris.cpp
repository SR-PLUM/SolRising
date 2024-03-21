// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Solaris.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

ASolaris::ASolaris()
{
 	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
}

void ASolaris::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASolaris::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASolaris::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASolaris::MoveForward(float Value)
{
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void ASolaris::MoveRight(float Value)
{
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void ASolaris::Turn(float Value)
{
	AddControllerYawInput(Value);
	//UE_LOG(LogTemp,Warning,TEXT("%f"),GetController()->GetControlRotation().Yaw)
}

void ASolaris::LookUp(float Value)
{
	AddControllerPitchInput(Value);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), GetController()->GetControlRotation().Pitch)
}

