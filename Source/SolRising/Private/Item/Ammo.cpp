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
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh1(TEXT("/Game/Blueprints/Item/SM_5_56mm_Mag"));
	EAT_5Mesh = Mesh1.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh2(TEXT("/Game/Blueprints/Item/SM_7_62mm_Mag"));
	EAT_7Mesh = Mesh2.Object;
}

void AAmmo::BeginPlay()
{
	Super::BeginPlay();

	int32 ammoType = FMath::RandRange(0, 1);

	switch (ammoType)
	{
	case (uint8)(E_AmmoType::EAT_5):

		if (EAT_5Mesh)
		{
			AmmoMesh->SetStaticMesh(EAT_5Mesh);
		}

		weight = 0.2 * count;
		break;
	case (uint8)(E_AmmoType::EAT_7):

		if (EAT_7Mesh)
		{
			AmmoMesh->SetStaticMesh(EAT_7Mesh);
		}

		weight = 0.4 * count;
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
