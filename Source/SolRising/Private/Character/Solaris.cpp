// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Solaris.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"

#include "Item/Item.h"
#include "Item/Gun.h"
#include "Item/Bag.h"
#include "Item/Ammo.h"
#include "Architecture/Door.h"

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

	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FPCamera->SetupAttachment(GetMesh());

	PickItemRange = CreateDefaultSubobject<USphereComponent>(TEXT("PickItemRange"));
	PickItemRange->SetupAttachment(RootComponent);

	Bag = CreateDefaultSubobject<ABag>(TEXT("Bag"));

	healthPoint = 100.f;
}

void ASolaris::BeginPlay()
{
	Super::BeginPlay();	
	
	PickItemRange->OnComponentBeginOverlap.AddDynamic(this, &ASolaris::OnItemBeginOverlap);
	PickItemRange->OnComponentEndOverlap.AddDynamic(this, &ASolaris::OnItemEndOverlap);
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
	AItem* pickedItem = nullptr;

	FHitResult CameraHit;
	auto bIsCameraHit = LineTracingMouse(CameraHit);

	if (bIsCameraHit)
	{
		auto CameraHitDoor = Cast<ADoor>(CameraHit.GetActor());
		if (CameraHitDoor)
		{
			if (GetDistanceTo(CameraHitDoor) <= 400.f)
			{
				CameraHitDoor->Open(GetActorLocation());
				return;
			}
		}

		auto CameraHitItem = Cast<AItem>(CameraHit.GetActor());
		if (CameraHitItem)
		{
			pickedItem = CameraHitItem;
		}
	}

	if (!pickedItem)
	{
		if (OverlappedItem.IsEmpty())
			return;

		pickedItem = OverlappedItem[0];
		OverlappedItem.Remove(pickedItem);
	}

	AGun* gun = Cast<AGun>(pickedItem);
	if (gun)
	{
		if (MainGun == nullptr)
		{
			MainGun = gun;
			MainGun->AttachMeshToSocket(GetMesh(), FName("RightHandIdleSocket"));
			MainGun->SetOwningCharacter(this);
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
			MainGun->AttachMeshToSocket(GetMesh(), FName("RightHandIdleSocket"));
			MainGun->SetOwningCharacter(this);
		}
	}

	ABag* bag = Cast<ABag>(pickedItem);
	if (bag)
	{
		if (Bag == nullptr || bag->GetMaxWeight() >= Bag->GetMaxWeight())
		{
			Bag = bag;
			UE_LOG(LogTemp, Warning, TEXT("새로 먹은 가방 최대 용량 : %f, 현재 가방용량 : %f"), Bag->GetMaxWeight(), Bag->currentWeight)
			Bag->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
			//TODO 소켓 장착
		}
	}

	AAmmo* ammo = Cast<AAmmo>(pickedItem);
	if (ammo && Bag != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ammo 이며 bag 있음"))
		if (Bag->CanPick(ammo->weight))
		{
			Bag->currentWeight += ammo->weight;
			Bag->AddItem(pickedItem);
			Bag->AddAmmoCount(ammo->ammoType_gen, ammo->count);

			ammo->Destroy();

			UE_LOG(LogTemp, Warning, TEXT("총알 획득 현재 용량 : %f / %f"), Bag->currentWeight, Bag->GetMaxWeight())
		}			
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("총알 획득 실패"))
		}
	}
	else if (ammo && Bag == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bag 없음"))
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

FVector ASolaris::GetCameraLocation()
{
	if (!ViewCamera)
		return FVector();

	return ViewCamera->GetComponentLocation();
}

FRotator ASolaris::GetCameraRotation()
{
	if(!ViewCamera)
		return FRotator();

	return ViewCamera->GetComponentRotation();
}

bool ASolaris::LineTracingMouse(FHitResult& CameraHit)
{
	const float TraceDistance = 10000.f;
	UWorld* World = GetWorld();

	if (World)
	{
		FVector StartCameraTrace;  //= GetCameraLocation();
		if (ViewCamera->IsActive())
		{
			StartCameraTrace = ViewCamera->GetComponentLocation();
		}
		else if (FPCamera->IsActive())
		{
			StartCameraTrace = FPCamera->GetComponentLocation();
		}
		
		FVector EndCameraTrace = StartCameraTrace + (GetCameraRotation().Vector() * TraceDistance);
		ECollisionChannel ECC_CameraHit = ECC_Visibility;
		bool bIsHitCamera = World->LineTraceSingleByChannel(CameraHit, StartCameraTrace, EndCameraTrace, ECC_CameraHit);
		if (bIsHitCamera)
		{
			DrawDebugLine(World, StartCameraTrace, EndCameraTrace, FColor::Red, false, 2.f);
			return true;
		}
	}

	return false;
}

void ASolaris::TogglePerspective()
{
	if (ViewCamera->IsActive() == true)
	{
		FPCamera->Activate();
		ViewCamera->Deactivate();
	}
	else if (ViewCamera->IsActive() == false)
	{
		ViewCamera->Activate();
		FPCamera->Deactivate();
	}
}

void ASolaris::Aiming()
{
	if (ViewCamera->IsActive() == true)
	{
		FPCamera->Activate();
		ViewCamera->Deactivate();

		IsAiming = true;
	}
	else if (ViewCamera->IsActive() == false)
	{
		ViewCamera->Activate();
		FPCamera->Deactivate();

		IsAiming = false;
	}
}

float ASolaris::GetHP()
{
	return healthPoint;
}

void ASolaris::SetHP(float setHP)
{
	healthPoint = setHP;
}

float ASolaris::GetCurrentGunDamage()
{
	if (MainGun)
		return MainGun->damage;
	else 
		return 0.f;
}

