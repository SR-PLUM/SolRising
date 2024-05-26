// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Gun.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"
#include "Projectile/AmmoProjectile.h"
#include "Character/Solaris.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"

#include "UI/Inventory.h"

AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	weight = 0;

	if (!RootComponent)
	{
		auto SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSceneComponent"));
		RootComponent = SceneComponent;
	}
	if (!GunMesh)
	{
		GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
		GunMesh->SetupAttachment(RootComponent);
	}
	if (!MuzzleLocation)
	{
		MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
		MuzzleLocation->SetupAttachment(GunMesh);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh1(TEXT("/Game/Blueprints/Item/StaticMesh/SM_M416"));
	M416Mesh = Mesh1.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh2(TEXT("/Game/Blueprints/Item/StaticMesh/SM_KA74U_Y"));
	AK74UMesh = Mesh2.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh3(TEXT("/Game/Blueprints/Item/StaticMesh/SM_AK47"));
	AK47Mesh = Mesh3.Object;

	itemName = FText::FromString("Gun");
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	auto gunName = FMath::RandRange(0, 2);

	switch (gunName)
	{
	case (uint8)(E_GunName::EGN_M416):

		if (M416Mesh && GunMesh)
		{
			GunMesh->SetStaticMesh(M416Mesh);
		}
		if (MuzzleLocation)
		{
			MuzzleLocation->SetRelativeLocation(FVector(0.f, 62.f, 10.5f));
		}

		currentGunName = E_GunName::EGN_M416;
		damage = M416Damage;
		RPM = M416RPM;
		verticalRecoil = M416VerticalRecoil;
		horizontalRecoil = M416HorizontalRecoil;
		FireSound = M416FireSound;
		FireAnimation = M416FireAnimation;
		break;
	case (uint8)(E_GunName::EGN_AK74U):

		if (AK74UMesh && GunMesh)
		{
			GunMesh->SetStaticMesh(AK74UMesh);
		}
		if (MuzzleLocation)
		{
			MuzzleLocation->SetRelativeLocation(FVector(0.f, 49.f, 8.f));
		}

		currentGunName = E_GunName::EGN_AK74U;
		damage = AK74UDamage;
		RPM = AK74URPM;
		verticalRecoil = AK74UVerticalRecoil;
		horizontalRecoil = AK74UHorizontalRecoil;
		FireSound = AK74UFireSound;
		FireAnimation = AK74UFireAnimation;
		break;
	case (uint8)(E_GunName::EGN_AK47):

		if (AK47Mesh && GunMesh)
		{
			GunMesh->SetStaticMesh(AK47Mesh);
		}
		if (MuzzleLocation)
		{
			MuzzleLocation->SetRelativeLocation(FVector(0.f, 65.f, 8.f));
		}

		currentGunName = E_GunName::EGN_AK47;
		damage = AK47Damage;
		RPM = AK47RPM;
		verticalRecoil = AK47VerticalRecoil;
		horizontalRecoil = AK47HorizontalRecoil;
		FireSound = AK47FireSound;
		FireAnimation = AK47FireAnimation;
		break;
	default:
		break;
	}

}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::SelectiveFire()
{
	switch (currentFireMethod)
	{
	case E_FireMethod::EFM_Semi_Auto:
		currentFireMethod = E_FireMethod::EFM_Full_Auto;
		break;
	case E_FireMethod::EFM_Full_Auto:
		currentFireMethod = E_FireMethod::EFM_Semi_Auto;
		break;
	default:
		break;
	}
}

void AGun::OnFire()
{
	loadedAmmo = 30;

	if (loadedAmmo <= 0 || isReloading == true || canFire == false) { return; }

	loadedAmmo -= 1;
	UE_LOG(LogTemp, Log, TEXT("Current Ammo : %d / %d"), loadedAmmo, remainAmmo);

	if (!OwningCharacter) { return; }

	if (OwningCharacter->InventoryWidget)
	{
		OwningCharacter->InventoryWidget->RefreshMainGunSlot(this);
	}

	//Fire Bullet
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FRotator SpawnRotation = GetActorRotation();
		FVector SpawnLocation = MuzzleLocation->GetComponentLocation();

		FHitResult CameraHit;
		bool bIsHitCamera = OwningCharacter->LineTracingMouse(CameraHit);

		if (bIsHitCamera)
		{
			SpawnRotation = (CameraHit.Location - MuzzleLocation->GetComponentLocation()).Rotation();
		}
		else if (OwningCharacter)
		{
			SpawnRotation = OwningCharacter->GetCameraRotation();
		}

		if (OwningCharacter->IsAiming == false)
		{
			auto yawBulletSpread = FMath::RandRange(-50, 50);
			auto pitchBulletSpread = FMath::RandRange(-50, 50);
			auto rollBulletSpread = FMath::RandRange(-50, 50);
			SpawnRotation += FRotator(yawBulletSpread / 10, pitchBulletSpread / 10, rollBulletSpread / 10);	//10을 변수로 변경
		}		

		FActorSpawnParameters SpawnParams;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(SpawnRotation.Quaternion());

		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		auto ammoProjectile = World->SpawnActor<AAmmoProjectile>(AmmoProjectileActor, SpawnTransform, SpawnParams);
		ammoProjectile->OwningCharacter = OwningCharacter;
	}

	//FireMontage
	OwningCharacter->PlayFireMontage();

	//Recoil
	auto yawRecoil = FMath::RandRange(-100, 100);
	auto pitchRecoil = FMath::RandRange(-200, -100);

	OwningCharacter->AddControllerYawInput(yawRecoil / 100);
	OwningCharacter->AddControllerPitchInput(pitchRecoil / 100);

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	GetWorld()->GetTimerManager().SetTimer(fireTimer, this, &AGun::FireCooldown, float(RPM / 60), false);
}

void AGun::Fire()
{
	switch (currentFireMethod)
	{
	case E_FireMethod::EFM_Semi_Auto:
		isFiring = true;
		OnFire();
		break;
	case E_FireMethod::EFM_Full_Auto:
		isFiring = true;
		OnFire();
		GetWorld()->GetTimerManager().SetTimer(fireTimer, this, &AGun::OnFire, float(RPM/30), true); // 0.*f : 연사율, true : 반복
		break;
	}
		
}

void AGun::FireCooldown()
{
	canFire = true;
}

void AGun::StopFire()
{
	isFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(fireTimer);
}

void AGun::Reroad()
{
	if (isReloading == false && loadedAmmo != magazine && remainAmmo != 0)
	{
		isReloading = true;
		UE_LOG(LogTemp, Log, TEXT("Start Reloading"));

		GetWorld()->GetTimerManager().SetTimer(reloadTimer, this, &AGun::ReloadDelay, 1.0f, false);

		if (remainAmmo <= 0 || loadedAmmo >= magazine) { return; }

		if (remainAmmo < (magazine - loadedAmmo))
		{
			loadedAmmo = loadedAmmo + remainAmmo;
			remainAmmo = 0;
		}
		else
		{
			remainAmmo = remainAmmo - (magazine - loadedAmmo);
			loadedAmmo = magazine;
		}

		UE_LOG(LogTemp, Log, TEXT("Reloading Complete\nCurrent Ammo : %d / %d"), loadedAmmo, remainAmmo);
	}
}

void AGun::ReloadDelay()
{
	isReloading = false;
	GetWorldTimerManager().ClearTimer(reloadTimer);
}

void AGun::Aiming()
{
}

void AGun::SetOwningCharacter(ASolaris* owningCharacter)
{
	OwningCharacter = owningCharacter;
}

int32 AGun::GetLoadedAmmo()
{
	return loadedAmmo;
}

float AGun::GetAmmoWeight()
{
	if (currentGunName == E_GunName::EGN_M416)
		return 0.2 * loadedAmmo;
	else
		return 0.4 * loadedAmmo;
}

int32 AGun::GetAmmoType()
{
	if (currentGunName == E_GunName::EGN_M416)
		return 0;
	else
		return 1;
}

FText AGun::GetGunName()
{
	FText gunName;

	switch (currentGunName)
	{
	case E_GunName::EGN_M416:
		gunName = FText::FromString("M416");
		break;
	case E_GunName::EGN_AK74U:
		gunName = FText::FromString("AK74U");
		break;
	case E_GunName::EGN_AK47:
		gunName = FText::FromString("AK47");
		break;
	case E_GunName::EGN_Null:
		gunName = FText::FromString("Null");
		break;
	default:
		break;
	}
	return gunName;
}
