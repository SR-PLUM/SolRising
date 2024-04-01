// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Bag.h"
#include "Components/SphereComponent.h"

#include <random>

ABag::ABag()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!BagMesh)
	{
		BagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BagMesh"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Blueprints/Item/Leather_Water_Pot_wgxobbm/S_Leather_Water_Pot_wgxobbm_lod0_Var1"));
		if (Mesh.Succeeded())
		{
			BagMesh->SetStaticMesh(Mesh.Object);
		}

		RootComponent = BagMesh;
	}
	if (!RootComponent)
	{
		auto SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSceneComponent"));
		SceneComponent->SetupAttachment(RootComponent);
	}

	if (!BagMesh)
	{
		BagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BagMesh"));
		BagMesh->SetupAttachment(RootComponent);

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("BagMeshPath"));
		if (Mesh.Succeeded())
		{
			BagMesh->SetStaticMesh(Mesh.Object);
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

		BagMesh->SetRelativeScale3D(FVector(3, 3, 3));
		maxWeight = 50;
		break;
	case static_cast<int>(E_BagType::EBT_MediumBag):

		BagMesh->SetRelativeScale3D(FVector(4, 4, 4));
		maxWeight = 100;
		break;
	case static_cast<int>(E_BagType::EBT_LargeBag):
		
		BagMesh->SetRelativeScale3D(FVector(5, 5, 5));
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
