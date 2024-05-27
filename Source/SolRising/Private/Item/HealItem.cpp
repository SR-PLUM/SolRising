// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HealItem.h"

AHealItem::AHealItem()
{
	PrimaryActorTick.bCanEverTick = true;

	if (!HealItemMesh)
	{
		HealItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealItemMesh"));

		/*static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("AmmoMeshPath"));
		if (Mesh.Succeeded())
		{
			HealItemMesh->SetStaticMesh(Mesh.Object);
		}*/

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

	itemName = FText::FromString("HealItem");
}

void AHealItem::BeginPlay()
{
	Super::BeginPlay();

	healItemType = FMath::RandRange(0, 1);

	switch (healItemType)
	{
		case (uint8)(E_HelaItemType::EHT_FirstAidKit) :
			if (FirstAidKitMesh)
			{
				HealItemMesh->SetStaticMesh(FirstAidKitMesh);
			}
			count = 1;
			individualWeight = 10;
			amountOfRecovery = 100;
			weight = individualWeight * count;
			itemName = FText::FromString("FirstAidKit");
			break;

		case (uint8)(E_HelaItemType::EHT_Bandage) :
			if (BandageMesh)
			{
				HealItemMesh->SetStaticMesh(BandageMesh);
			}
			count = 5;
			individualWeight = 2;
			amountOfRecovery = 10;
			weight = individualWeight * count;
			itemName = FText::FromString("Bandage");
			break;

		default:
			break;
	}
}

void AHealItem::Tick(float DeltaTime)
{
}

float AHealItem::GetAmountOfRecovery()
{
	return amountOfRecovery;
}
