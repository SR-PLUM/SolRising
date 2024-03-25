// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Gun.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"
#include "Projectile/AmmoProjectile.h"
#include "Character/Solaris.h"
#include "Camera/CameraComponent.h"

#include <random>

AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 2);

	if (!GunMesh)
	{
		GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("GunMeshPath"));
		if (Mesh.Succeeded())
		{
			GunMesh->SetStaticMesh(Mesh.Object);
		}

		RootComponent = GunMesh;
	}
	if (!RootComponent)
	{
		auto SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSceneComponent"));
		SceneComponent->SetupAttachment(RootComponent);
	}
	if (!InteractionComponent)
	{
		InteractionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		InteractionComponent->InitSphereRadius(5.0f);
		InteractionComponent->SetupAttachment(RootComponent);
		//RootComponent = InteractionComponent;
	}
	if (!MuzzleLocation)
	{
		MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
		MuzzleLocation->SetupAttachment(RootComponent);
	}

	switch (dis(gen))	//TODO case를 enum값으로 바꾸기 EGN
	{
	case 0:
		damage = M416Damage;
		RPM = M416RPM;
		verticalRecoil = M416VerticalRecoil;
		horizontalRecoil = M416HorizontalRecoil;
		FireSound = M416FireSound;
		FireAnimation = M416FireAnimation;
		break;
	case 1:
		damage = SCARDamage;
		RPM = SCARRPM;
		verticalRecoil = SCARVerticalRecoil;
		horizontalRecoil = SCARHorizontalRecoil;
		FireSound = SCARFireSound;
		FireAnimation = SCARFireAnimation;
		break;
	case 2:
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

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::Picked()
{
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

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FRotator SpawnRotation = GetActorRotation();
		FVector SpawnLocation = MuzzleLocation->GetComponentLocation();

		//Tracing Camera Aim
		FHitResult CameraHit;
		FVector StartCameraTrace = OwningCharacter->GetCameraLocation();
		FVector EndCameraTrace = StartCameraTrace + (OwningCharacter->GetCameraRotation().Vector() * TraceDistance);
		ECollisionChannel ECC_CameraHit = ECC_Visibility;
		bool bIsHitCamera = World->LineTraceSingleByChannel(CameraHit, StartCameraTrace, EndCameraTrace, ECC_CameraHit);
		if (bIsHitCamera)
		{
			SpawnRotation = (CameraHit.Location - MuzzleLocation->GetComponentLocation()).Rotation();
			DrawDebugLine(World, StartCameraTrace, EndCameraTrace, FColor::Red, false, 2.f);
		}

		FActorSpawnParameters SpawnParams;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(SpawnRotation.Quaternion());

		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		World->SpawnActor<AAmmoProjectile>(AmmoProjectileActor, SpawnTransform, SpawnParams);
	}

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

void AGun::AttachMeshToSocket(USceneComponent* InParent, const FName& SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	GunMesh->AttachToComponent(InParent, TransformRules, SocketName);
}

void AGun::SetOwningCharacter(ASolaris* owningCharacter)
{
	OwningCharacter = owningCharacter;
}
