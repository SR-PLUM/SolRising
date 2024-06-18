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

	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh1(TEXT("/Game/Blueprints/Item/StaticMesh/SM_Level_1_Vest"));
	Lv1VestMesh = Mesh1.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh2(TEXT("/Game/Blueprints/Item/StaticMesh/SM_Level_2_Vest"));
	Lv2VestMesh = Mesh2.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh3(TEXT("/Game/Blueprints/Item/StaticMesh/SM_Level_3_Vest"));
	Lv3VestMesh = Mesh3.Object;

	FString Lv1VestPath("/Game/Blueprints/UI/Widget_Images/LV1_Vest");
	FString Lv2VestPath("/Game/Blueprints/UI/Widget_Images/LV2_Vest");
	FString Lv3VestPath("/Game/Blueprints/UI/Widget_Images/LV3_Vest");

	Lv1VestImg = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Lv1VestPath));
	Lv2VestImg = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Lv2VestPath));
	Lv3VestImg = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Lv3VestPath));

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
		defense = 0.8;

		itemImg = Lv1VestImg;
		itemName = FText::FromString("Lv1_Vest");

		break;
		case (uint8)(E_BulletproofVestType::EBVT_LV2) :

			if (Lv2VestMesh && BulletproofVestMesh)
			{
				BulletproofVestMesh->SetStaticMesh(Lv2VestMesh);
			}
		defense = 0.7;

		itemImg = Lv2VestImg;
		itemName = FText::FromString("Lv2_Vest");

		break;
		case (uint8)(E_BulletproofVestType::EBVT_LV3) :

			if (Lv3VestMesh && BulletproofVestMesh)
			{
				BulletproofVestMesh->SetStaticMesh(Lv3VestMesh);
			}
		defense = 0.5;

		itemImg = Lv3VestImg;
		itemName = FText::FromString("Lv3_Vest");

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
