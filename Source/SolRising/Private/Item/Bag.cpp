// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Bag.h"
#include "Components/SphereComponent.h"

#include <random>

ABag::ABag()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	if (!BagMesh)
	{
		BagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BagMesh"));
		BagMesh->SetupAttachment(InteractionComponent);

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("BagMeshPath"));
		if (Mesh.Succeeded())
		{
			BagMesh->SetStaticMesh(Mesh.Object);
		}
	}

	maxWeight = baseMaxWeight;
}

// Called when the game starts or when spawned
void ABag::BeginPlay()
{
	Super::BeginPlay();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 2);

	int bagType_gen = dis(gen);

	switch (bagType_gen)
	{
	case static_cast<int>(E_BagType::EBT_SmallBag):
		maxWeight = 50;
		break;
	case static_cast<int>(E_BagType::EBT_MediumBag):
		maxWeight = 100;
		break;
	case static_cast<int>(E_BagType::EBT_LargeBag):
		maxWeight = 150;
		break;
	default:
		break;
	}
}

// Called every frame
void ABag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABag::GetMaxWeight()
{
	return maxWeight;
}

bool ABag::CanPick(float itemWeight)
{
	if (itemWeight <= maxWeight - currentWeight)
		return true;
	else
		return false;
}

void ABag::AddItem(AItem* item)
{
	havingItems.Add(item);
}

void ABag::AddAmmoCount(int AmmoType, int AmmoCount)
{
	currentAmmoCount[AmmoType] += AmmoCount;
}
