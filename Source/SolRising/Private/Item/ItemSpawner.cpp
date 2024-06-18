// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemSpawner.h"

#include "Item/Ammo.h"
#include "Item/Bag.h"
#include "Item/Gun.h"

// Sets default values
AItemSpawner::AItemSpawner()
{
	static ConstructorHelpers::FObjectFinder<UClass> AmmoRef(TEXT("Blueprint'/Game/Blueprints/Item/BP_Ammo.BP_Ammo_C'"));
	if (AmmoRef.Object)
	{
		AmmoBP = (UClass*)AmmoRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UClass> BagRef(TEXT("Blueprint'/Game/Blueprints/Item/BP_Bag.BP_Bag_C'"));
	if (BagRef.Object)
	{
		BagBP = (UClass*)BagRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UClass> GunRef(TEXT("Blueprint'/Game/Blueprints/Item/BP_Gun.BP_Gun_C'"));
	if (GunRef.Object)
	{
		GunBP = (UClass*)GunRef.Object;
	}
}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	int32 ItemType = FMath::RandRange(0, NumberOfItemType - 1);

	switch (ItemType)
	{
	case (uint8)E_SpawnItemType::ESIT_Ammo:
		SpawnAmmo(GetActorLocation());
		break;
	case (uint8)E_SpawnItemType::ESIT_Bag:
		SpawnBag();
		break;
	case (uint8)E_SpawnItemType::ESIT_Gun:
		SpawnGun();
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("ItemSpawner::ItemType, Out of Range"));
		break;
	}
}

AAmmo* AItemSpawner::SpawnAmmo(FVector Location)
{
	AAmmo* SpawnedAmmo = nullptr;

	auto World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.Owner = this;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(GetActorQuat());

		SpawnedAmmo = World->SpawnActor<AAmmo>(AmmoBP, SpawnTransform, SpawnParameter);
	}

	return SpawnedAmmo;
}

void AItemSpawner::SpawnBag()
{
	auto World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.Owner = this;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation());
		SpawnTransform.SetRotation(GetActorQuat());

		World->SpawnActor<ABag>(BagBP, SpawnTransform, SpawnParameter);
	}
}

void AItemSpawner::SpawnGun()
{
	auto World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.Owner = this;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation());
		SpawnTransform.SetRotation(GetActorQuat());

		auto SpawnedGun = World->SpawnActor<AGun>(GunBP, SpawnTransform, SpawnParameter);
		if (SpawnedGun)
		{
			auto SpawnedAmmo = SpawnAmmo(GetActorLocation() + FVector(0, 50, 0));
			if (SpawnedAmmo)
			{
				auto GunName = SpawnedGun->currentGunName;
				if (GunName == E_GunName::EGN_AK47 || GunName == E_GunName::EGN_AK74U)
				{
					SpawnedAmmo->ChangeAmmoType(E_AmmoType::EAT_7);
				}
				else if (GunName == E_GunName::EGN_M416)
				{
					SpawnedAmmo->ChangeAmmoType(E_AmmoType::EAT_5);
				}
			}
		}
	}
}
