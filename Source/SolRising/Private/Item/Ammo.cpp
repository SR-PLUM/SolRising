// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Ammo.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"

#include <random>

AAmmo::AAmmo()
{
	PrimaryActorTick.bCanEverTick = true;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 1);

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
	if (!AmmoMesh)
	{
		AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
		AmmoMesh->SetupAttachment(InteractionComponent);

		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("AmmoMeshPath"));
		if (Mesh.Succeeded())
		{
			AmmoMesh->SetStaticMesh(Mesh.Object);
		}
	}

	ammoType_gen = dis(gen);

	switch (ammoType_gen)
	{
	case static_cast<int>(E_AmmoType::EAT_5):
		weight = 0.2 * count;
		break;
	case static_cast<int>(E_AmmoType::EAT_7):
		weight = 0.4 * count;
		break;
	default:
		break;
	}
}

void AAmmo::BeginPlay()
{
}

void AAmmo::Tick(float DeltaTime)
{
}