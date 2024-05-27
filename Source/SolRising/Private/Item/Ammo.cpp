// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Ammo.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"

AAmmo::AAmmo()
{
	PrimaryActorTick.bCanEverTick = true;

	if (!AmmoMesh)
	{
		AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));

		/*static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("AmmoMeshPath"));
		if (Mesh.Succeeded())
		{
			AmmoMesh->SetStaticMesh(Mesh.Object);
		}*/

		RootComponent = AmmoMesh;
	}
	if (!RootComponent)
	{
		auto SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSceneComponent"));
		SceneComponent->SetupAttachment(RootComponent);
	}
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh1(TEXT("/Game/Blueprints/Item/StaticMesh/SM_5_56mm_Mag"));
	EAT_5Mesh = Mesh1.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh2(TEXT("/Game/Blueprints/Item/StaticMesh/SM_7_62mm_Mag"));
	EAT_7Mesh = Mesh2.Object;

	itemName = FText::FromString("Ammo");
}

void AAmmo::BeginPlay()
{
	Super::BeginPlay();

	count = 20;

	int32 ammoType = FMath::RandRange(0, 1);

	switch (ammoType)
	{
	case (uint8)(E_AmmoType::EAT_5):
		if (EAT_5Mesh)
		{
			AmmoMesh->SetStaticMesh(EAT_5Mesh);
		}
		individualWeight = 0.2;
		weight = 0.2 * count;
		itemName = FText::FromString("5.56mm Ammo");
		break;
	case (uint8)(E_AmmoType::EAT_7):

		if (EAT_7Mesh)
		{
			AmmoMesh->SetStaticMesh(EAT_7Mesh);
		}
		individualWeight = 0.4;
		weight = individualWeight * count;
		itemName = FText::FromString("7.62mm Ammo");
		break;
	default:
		break;
	}
}

void AAmmo::Tick(float DeltaTime)
{
}

void AAmmo::ChangeAmmoType(E_AmmoType EAT)
{
	if (EAT == E_AmmoType::EAT_5)
	{
		if(EAT_5Mesh && AmmoMesh)
			AmmoMesh->SetStaticMesh(EAT_5Mesh);
		weight = 0.2 * count;
	}
	else if (EAT == E_AmmoType::EAT_7)
	{
		if (EAT_7Mesh && AmmoMesh)
			AmmoMesh->SetStaticMesh(EAT_7Mesh);
		weight = 0.4 * count;
	}
}
