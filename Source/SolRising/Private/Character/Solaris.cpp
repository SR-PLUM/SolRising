// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Solaris.h"

#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

#include "Item/Item.h"
#include "Item/Gun.h"

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

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetupAttachment(GetRootComponent());
}

void ASolaris::BeginPlay()
{
	Super::BeginPlay();
	
	if (SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASolaris::OnItemBeginOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ASolaris::OnItemEndOverlap);
	}
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

void ASolaris::Pick()
{
	if (OverlappedItem.IsEmpty())
		return;

	AItem* pickedItem = OverlappedItem[0];
	OverlappedItem.Remove(pickedItem);

	AGun* gun = Cast<AGun>(pickedItem);
	if (gun)
	{
		if (MainGun == nullptr)
		{
			MainGun = gun;
			MainGun->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
			UE_LOG(LogTemp, Warning, TEXT("총 장착"))
		}
		else if (SubGun == nullptr)
		{
			SubGun = gun;
			//TODO 소켓에 장착
		}
		else
		{
			// TODO 
			// 메인건에 장전되어 있는 총알 수거
			// 메인건을 바닥으로 버림
			// OverlappedItem에 메인건 추가
			MainGun = gun;
			MainGun->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
		}
	}
}

void ASolaris::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AItem* item = Cast<AItem>(OtherActor))
	{
		OverlappedItem.Add(item);
	}
}

void ASolaris::OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AItem* item = Cast<AItem>(OtherActor))
	{
		OverlappedItem.Remove(item);
	}
}

AGun* ASolaris::GetMainGun()
{
	if (MainGun)
		return MainGun;

	return nullptr;
}

