// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Bag.h"
#include "Components/SphereComponent.h"

ABag::ABag()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!BagMesh)
	{
		BagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BagMesh"));

		RootComponent = BagMesh;
	}
	if (!RootComponent)
	{
		auto SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSceneComponent"));
		SceneComponent->SetupAttachment(RootComponent);
	}

	maxWeight = baseMaxWeight;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh1(TEXT("/Game/Blueprints/Item/StaticMesh/SM_Level_1_Bag"));
	Lv1BagMesh = Mesh1.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh2(TEXT("/Game/Blueprints/Item/StaticMesh/SM_Level_2_Bag"));
	Lv2BagMesh = Mesh2.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh3(TEXT("/Game/Blueprints/Item/StaticMesh/SM_Level_3_Bag"));
	Lv3BagMesh = Mesh3.Object;	
}

// Called when the game starts or when spawned
void ABag::BeginPlay()
{
	Super::BeginPlay();

	auto bagType = FMath::RandRange(0, 2);

	switch (bagType)
	{
	case (uint8)(E_BagType::EBT_SmallBag):

		if (Lv1BagMesh && BagMesh)
		{
			BagMesh->SetStaticMesh(Lv1BagMesh);
		}

		BagMesh->SetRelativeScale3D(FVector(3, 3, 3));
		maxWeight = 50;
		break;
	case (uint8)(E_BagType::EBT_MediumBag):

		if (Lv2BagMesh && BagMesh)
		{
			BagMesh->SetStaticMesh(Lv2BagMesh);
		}

		BagMesh->SetRelativeScale3D(FVector(4, 4, 4));
		maxWeight = 100;
		break;
	case (uint8)(E_BagType::EBT_LargeBag):
		
		if (Lv3BagMesh && BagMesh)
		{
			BagMesh->SetStaticMesh(Lv3BagMesh);
		}

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

void ABag::AttachMeshToSocket(USceneComponent* InParent, const FName& SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	RootComponent->AttachToComponent(InParent, TransformRules, SocketName);
}
