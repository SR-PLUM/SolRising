// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Solaris.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"

#include "Item/Item.h"
#include "Item/Gun.h"
#include "Item/Bag.h"
#include "Item/BulletproofVest.h"
#include "Item/Ammo.h"
#include "Item/HealItem.h"
#include "Architecture/Door.h"
#include "UI/Inventory.h"

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

	FPCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("FPCameraBoom"));
	FPCameraBoom->SetupAttachment(GetMesh());
	FPCameraBoom->TargetArmLength = 10.f;

	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FPCamera->SetupAttachment(FPCameraBoom);

	PickItemRange = CreateDefaultSubobject<USphereComponent>(TEXT("PickItemRange"));
	PickItemRange->SetupAttachment(RootComponent);

	currentHP = 50.f;

	ConstructorHelpers::FClassFinder<UUserWidget> InventoryBPClass(TEXT("/Game/Blueprints/UI/WBP_Inventory"));
	if (InventoryBPClass.Class != nullptr)
	{
		InventoryWidgetClass = InventoryBPClass.Class;
	}
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

void ASolaris::Interaction()
{
	AItem* pickedItem = nullptr;

	FHitResult CameraHit;
	auto bIsCameraHit = LineTracingMouse(CameraHit);

	//Interaction Using Mouse Cursor
	if (bIsCameraHit)
	{
		//Door Action
		auto CameraHitDoor = Cast<ADoor>(CameraHit.GetActor());
		if (CameraHitDoor)
		{
			if (GetDistanceTo(CameraHitDoor) <= 400.f)
			{
				CameraHitDoor->Open(GetActorLocation());
				return;
			}
		}

		//Pick Action Using Mouse Cursor
		auto CameraHitItem = Cast<AItem>(CameraHit.GetActor());
		if (CameraHitItem)
		{
			pickedItem = CameraHitItem;
		}
	}

	Pick(pickedItem);
}

void ASolaris::Pick(AItem* pickedItem)
{
	//If Can't Detect Any Item at Mouse Cursor
	//Check Item Near by Solaris
	if (!pickedItem)
	{
		if (OverlappedItem.IsEmpty())
			return;

		pickedItem = OverlappedItem[0];
		//OverlappedItem.Remove(pickedItem);
	}

	//If PickedItem is Gun
	AGun* gun = Cast<AGun>(pickedItem);
	if (gun)
	{
		if (MainGun == nullptr)
		{
			SetMainGun(gun);

			CurrentGun = MainGun;
		}
		else if (SubGun == nullptr)
		{
			SetSubGun(gun);

			UE_LOG(LogTemp, Warning, TEXT("서브건 장착"));
		}
		else
		{
			// TODO 
			// 메인건에 장전되어 있는 총알 수거
			if (GetMainGun()->GetLoadedAmmo() > 0)
			{
				if (CanPick(GetMainGun()->GetAmmoWeight()))
				{
					CurrentWeight += GetMainGun()->GetAmmoWeight();
					currentAmmoCount[GetMainGun()->GetAmmoType()] += GetMainGun()->GetLoadedAmmo();
				}
			}
			
			SetMainGun(gun);
		}

		OverlappedItem.Remove(pickedItem);
		return;
	}

	//If PickedItem is Bag
	ABag* bag = Cast<ABag>(pickedItem);
	if (bag)
	{
		if (MaxWeight <= bag->GetMaxWeight())
		{
			Bag = bag;
			MaxWeight = bag->GetMaxWeight();
			Bag->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));

			UE_LOG(LogTemp, Warning, TEXT("새로 먹은 가방 최대 용량 : %f, 현재 가방용량 : %f"), Bag->GetMaxWeight(), CurrentWeight);

			OverlappedItem.Remove(pickedItem);
		}

		return;
	}

	//If PickedItem is BulletproofVest
	ABulletproofVest* vest = Cast<ABulletproofVest>(pickedItem);
	if (vest)
	{
		CurrentVestDefence = vest->GetDefence();

		UE_LOG(LogTemp, Warning, TEXT("조끼 방어력 : %f"), vest->GetDefence());

		vest->Destroy();
		OverlappedItem.Remove(vest);

		return;
	}

	//If PickedItem is Ammo
	AAmmo* ammo = Cast<AAmmo>(pickedItem);
	if (ammo)
	{
		if (CanPick(ammo->weight))
		{
			CurrentWeight += ammo->weight;
			currentAmmoCount[ammo->ammoType_gen] += ammo->count;

			UE_LOG(LogTemp, Warning, TEXT("총알 획득 현재 용량 : %f / %f"), CurrentWeight, MaxWeight);

			ammo->Destroy();
			OverlappedItem.Remove(ammo);

			if (InventoryWidget)
			{
				InventoryWidget->AddInventory(ammo);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("총알 획득 실패"));
		}

		return;
	}

	AHealItem* healItem = Cast<AHealItem>(pickedItem);
	if(healItem)
	{
		if (healItem->healItemType == (uint8)(E_HelaItemType::EHT_FirstAidKit))
		{
			UE_LOG(LogTemp, Warning, TEXT("구급상자 획득"));
		}
		else if (healItem->healItemType == (uint8)(E_HelaItemType::EHT_Bandage))
		{
			UE_LOG(LogTemp, Warning, TEXT("붕대 획득"));
		}

		healItem->Destroy();
		OverlappedItem.Remove(healItem);

		return;
	}
}

void ASolaris::Kneel()
{
	if (GroundPose == E_GroundPose::EGP_Kneel)
	{
		GroundPose = E_GroundPose::EGP_Standing;
	}
	else
	{
		GroundPose = E_GroundPose::EGP_Kneel;
	}
}

void ASolaris::Prone()
{
	if (GroundPose == E_GroundPose::EGP_Prone)
	{
		GroundPose = E_GroundPose::EGP_Standing;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	else
	{
		GroundPose = E_GroundPose::EGP_Prone;
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
}

bool ASolaris::Inventory()
{
	if (!InventoryWidget)
	{
		if (InventoryWidgetClass)
		{
			InventoryWidget = Cast<UInventory>(CreateWidget(GetWorld(), InventoryWidgetClass));
			if (InventoryWidget)
			{
				InventoryWidget->Owner = this;

				for (auto item : OverlappedItem)
				{
					InventoryWidget->AddList(item);
				}

				InventoryWidget->AddToViewport();
				IsInventoryOpen = true;

				return true;
			}
		}

		return false;
	}

	if (IsInventoryOpen)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		IsInventoryOpen = false;

		return false;
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		IsInventoryOpen = true;

		return true;
	}
}

bool ASolaris::CanPick(float itemWeight)
{
	if (itemWeight <= MaxWeight - CurrentWeight)
		return true;
	else
		return false;
}

void ASolaris::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AItem* item = Cast<AItem>(OtherActor))
	{
		OverlappedItem.Add(item);

		if (InventoryWidget)
		{
			InventoryWidget->AddList(item);
		}
	}
}

void ASolaris::OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AItem* item = Cast<AItem>(OtherActor))
	{
		OverlappedItem.Remove(item);

		if (InventoryWidget)
		{
			InventoryWidget->RemoveList(item);
		}
	}
}

void ASolaris::SetMainGun(AGun* gun)
{
	MainGun = gun;

	MainGun->AttachMeshToSocket(GetMesh(), FName("RightHandIdleSocket"));
	MainGun->SetOwningCharacter(this);

	if (InventoryWidget)
	{
		InventoryWidget->RefreshMainGunSlot(MainGun);
	}

	UE_LOG(LogTemp, Warning, TEXT("메인건 장착"));
}

AGun* ASolaris::GetMainGun()
{
	if (MainGun)
		return MainGun;

	return nullptr;
}

void ASolaris::SetSubGun(AGun* gun)
{
	SubGun = gun;

	gun->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));

	if (InventoryWidget)
	{
		InventoryWidget->RefreshSubGunSlot(SubGun);
	}
}

AGun* ASolaris::GetSubGun()
{
	if (SubGun)
		return SubGun;

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
	if(!ViewCamera || !FPCamera)
		return FRotator();

	if (ViewCamera->IsActive())
		return ViewCamera->GetComponentRotation();
	else if (FPCamera->IsActive())
		return FPCamera->GetComponentRotation();
	else
		return FRotator();
}

bool ASolaris::LineTracingMouse(FHitResult& CameraHit)
{
	const float TraceDistance = 10000.f;
	UWorld* World = GetWorld();

	if (World)
	{
		FVector StartCameraTrace;
		if (ViewCamera && FPCamera)
		{
			if (IsAiming == false)
			{
				StartCameraTrace = ViewCamera->GetComponentLocation();
			}
			else if (IsAiming == true)
			{
				StartCameraTrace = FPCamera->GetComponentLocation();
			}
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
	if (IsAiming == false)
	{
		FPCamera->Activate();
		ViewCamera->Deactivate();
	}
	else if (IsAiming == true)
	{
		ViewCamera->Activate();
		FPCamera->Deactivate();
	}
}

void ASolaris::Aiming()
{
	if (IsAiming == false)
	{
		FPCamera->Activate();
		ViewCamera->Deactivate();

		IsAiming = true;
	}
	else if (IsAiming == true)
	{
		ViewCamera->Activate();
		FPCamera->Deactivate();

		IsAiming = false;
	}
}

void ASolaris::PlayFireMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayFireMonatge"));
		AnimInstance->Montage_Play(FireMontage);
		AnimInstance->Montage_JumpToSection("IdleFire", FireMontage);
	}
}

void ASolaris::RefreshInventory()
{

}

float ASolaris::GetCurrentVestDefence()
{
	return CurrentVestDefence;
}

void ASolaris::TakeDamege(bool isHeadShot, float damage, float reduceDamage)
{
	if (isHeadShot == true)
	{
		currentHP -= damage * 2;
	}
	else
	{
		currentHP -= damage * reduceDamage;
	}
}

void ASolaris::HealHP(float AmountOfRecovery)
{
	if (currentHP + AmountOfRecovery > maxHP)
	{
		currentHP = maxHP;
	}
	else
	{
		currentHP += AmountOfRecovery;
	}	
}

void ASolaris::UseItem(AItem* item)
{
	AHealItem* healItem = Cast<AHealItem>(item);
	if (healItem)
	{
		HealHP(healItem->GetAmountOfRecovery());	
	}
}

float ASolaris::GetHP()
{
	return currentHP;
}

float ASolaris::GetCurrentGunDamage()
{
	if (CurrentGun)
		return CurrentGun->damage;
	else 
		return 0.f;
}

void ASolaris::AttachMainGun()
{
	if (SubGun)
	{
		SubGun->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
	MainGun->AttachMeshToSocket(GetMesh(), FName("RightHandIdleSocket"));
}

void ASolaris::AttachSubGun()
{
	if (MainGun)
	{
		MainGun->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
	SubGun->AttachMeshToSocket(GetMesh(), FName("RightHandIdleSocket"));
}

