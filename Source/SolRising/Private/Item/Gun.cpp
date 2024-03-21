// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Gun.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"

#include <random>

AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 2);

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSceneComponent"));
	}
	if (!InteractionComponent)
	{
		InteractionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		InteractionComponent->InitSphereRadius(5.0f);
		RootComponent = InteractionComponent;
	}
	if (!GunMesh)
	{
		GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
		GunMesh->SetupAttachment(InteractionComponent);

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("GunMeshPath"));
		if (Mesh.Succeeded())
		{
			GunMesh->SetStaticMesh(Mesh.Object);
		}
	}

	switch (dis(gen))
	{
	case E_GunName::M416:
		damage = M416Damage;
		RPM = M416RPM;
		verticalRecoil = M416VerticalRecoil;
		horizontalRecoil = M416HorizontalRecoil;
		FireSound = M416FireSound;
		FireAnimation = M416FireAnimation;
		break;
	case E_GunName::SCAR:
		damage = SCARDamage;
		RPM = SCARRPM;
		verticalRecoil = SCARVerticalRecoil;
		horizontalRecoil = SCARHorizontalRecoil;
		FireSound = SCARFireSound;
		FireAnimation = SCARFireAnimation;
		break;
	case E_GunName::AK47:
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
	case AGun::Semi_Auto:
		currentFireMethod = E_FireMethod::Full_Auto;
		break;
	case AGun::Full_Auto:
		currentFireMethod = E_FireMethod::Semi_Auto;
		break;
	default:
		break;
	}
}

void AGun::OnFire()
{
	if (loadedAmmo <= 0 || isReloading == true || canFire == false) { return; }

	loadedAmmo -= 1;
	UE_LOG(LogTemp, Log, TEXT("Current Ammo : %d / %d"), loadedAmmo, remainAmmo);

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FRotator SpawnRotation = GetActorRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
		FHitResult hit;


		FVector Start = SpawnLocation;
		FVector End;

		End = Start + (SpawnRotation.Vector() * TraceDistance);

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		bool bHit = World->LineTraceSingleByChannel(hit, Start, End, ECC_Visibility, TraceParams);

		DrawDebugLine(World, Start, End, FColor::Blue, false, 2.0f);

		if (bHit)
		{
			UE_LOG(LogTemp, Log, TEXT("%s"), *hit.GetActor()->GetName());
			DrawDebugBox(World, hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	//if (FireAnimation != nullptr)
	//{
	//	// Get the animation object for the arms mesh
	//	UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
	//	if (AnimInstance != nullptr)
	//	{
	//		AnimInstance->Montage_Play(FireAnimation, 1.f);
	//	}
	//}

	GetWorld()->GetTimerManager().SetTimer(fireTimer, this, &AGun::FireCooldown, float(RPM / 60), false);
}

void AGun::Fire()
{
	switch (currentFireMethod)
	{
	case E_FireMethod::Semi_Auto:
		isFiring = true;
		OnFire();
		break;
	case E_FireMethod::Full_Auto:
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
