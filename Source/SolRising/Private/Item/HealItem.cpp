// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HealItem.h"

AHealItem::AHealItem()
{
	PrimaryActorTick.bCanEverTick = true;

	if (!HealItemMesh)
	{
		HealItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealItemMesh"));

		RootComponent = HealItemMesh;
	}
	if (!RootComponent)
	{
		auto SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSceneComponent"));
		SceneComponent->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh1(TEXT("/Game/Blueprints/Item/StaticMesh/SM_FirstAidKit"));
	FirstAidKitMesh = Mesh1.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh2(TEXT("/Game/Blueprints/Item/StaticMesh/SM_Bandage"));
	BandageMesh = Mesh2.Object;

	FString FirstAidKitPath("/Game/Blueprints/UI/Widget_Images/FirstAidKit");
	FString BandagePath("/Game/Blueprints/UI/Widget_Images/Bandage");

	FirstAidKitImg = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *FirstAidKitPath));
	BandageImg = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *BandagePath));

	itemName = FText::FromString("HealItem");
}

void AHealItem::BeginPlay()
{
	Super::BeginPlay();

	healItemType = FMath::RandRange(0, 1);

	switch (healItemType)
	{
		case (uint8)(E_HealItemType::EHT_FirstAidKit) :
			if (FirstAidKitMesh)
			{
				HealItemMesh->SetStaticMesh(FirstAidKitMesh);
			}
			count = 1;
			individualWeight = 10;
			weight = individualWeight * count;

			itemImg = FirstAidKitImg;
			itemName = FText::FromString("FirstAidKit");
			break;

		case (uint8)(E_HealItemType::EHT_Bandage) :
			if (BandageMesh)
			{
				HealItemMesh->SetStaticMesh(BandageMesh);
			}
			count = 5;
			individualWeight = 2;
			weight = individualWeight * count;

			itemImg = BandageImg;
			itemName = FText::FromString("Bandage");
			break;

		default:
			break;
	}
}

void AHealItem::Tick(float DeltaTime)
{
}

void AHealItem::ChangeHealItemType(E_HealItemType type)
{
	if (type == E_HealItemType::EHT_Bandage)
	{
		if (BandageMesh)
		{
			HealItemMesh->SetStaticMesh(BandageMesh);
		}
		individualWeight = 2;
		weight = individualWeight * count;

		itemImg = BandageImg;
		itemName = FText::FromString("Bandage");

		healItemType = (int32) E_HealItemType::EHT_Bandage;
	}
	if (type == E_HealItemType::EHT_FirstAidKit)
	{
		if (FirstAidKitMesh)
		{
			HealItemMesh->SetStaticMesh(FirstAidKitMesh);
		}
		individualWeight = 10;
		weight = individualWeight * count;

		itemImg = FirstAidKitImg;
		itemName = FText::FromString("FirstAidKit");

		healItemType = (int32)E_HealItemType::EHT_FirstAidKit;
	}
}
