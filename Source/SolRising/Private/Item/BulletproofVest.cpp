// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BulletproofVest.h"

ABulletproofVest::ABulletproofVest()
{
	PrimaryActorTick.bCanEverTick = true;

	if (!BulletproofVestMesh)
	{
		BulletproofVestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletproofVestMesh"));

		RootComponent = BulletproofVestMesh;
	}
	if (!RootComponent)
	{
		auto SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSceneComponent"));
		SceneComponent->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh1(TEXT("/Game/Blueprints/Item/StaticMesh/SM_Level_1_Bag"));
	Lv1VestMesh = Mesh1.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh2(TEXT("/Game/Blueprints/Item/StaticMesh/SM_Level_2_Bag"));
	Lv2VestMesh = Mesh2.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh3(TEXT("/Game/Blueprints/Item/StaticMesh/SM_Level_3_Bag"));
	Lv3VestMesh = Mesh3.Object;

	itemName = FText::FromString("BulletproofVest");
}

void ABulletproofVest::BeginPlay()
{
	Super::BeginPlay();

	auto vestType = FMath::RandRange(0, 2);

	switch (vestType)
	{
		case (uint8)(E_BulletproofVestType::EBVT_LV1) :

			if (Lv1VestMesh && BulletproofVestMesh)
			{
				BulletproofVestMesh->SetStaticMesh(Lv1VestMesh);
			}

		BulletproofVestMesh->SetRelativeScale3D(FVector(3, 3, 3));
		defense = 0.8;
		break;
		case (uint8)(E_BulletproofVestType::EBVT_LV2) :

			if (Lv2VestMesh && BulletproofVestMesh)
			{
				BulletproofVestMesh->SetStaticMesh(Lv2VestMesh);
			}

		BulletproofVestMesh->SetRelativeScale3D(FVector(4, 4, 4));
		defense = 0.7;
		break;
		case (uint8)(E_BulletproofVestType::EBVT_LV3) :

			if (Lv3VestMesh && BulletproofVestMesh)
			{
				BulletproofVestMesh->SetStaticMesh(Lv3VestMesh);
			}

		BulletproofVestMesh->SetRelativeScale3D(FVector(5, 5, 5));
		defense = 0.5;
		break;
		default:
			break;
	}
}

void ABulletproofVest::Tick(float DeltaTime)
{
}

float ABulletproofVest::GetDefence()
{
	return defense;
}
