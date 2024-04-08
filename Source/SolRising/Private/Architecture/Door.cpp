// Fill out your copyright notice in the Description page of Project Settings.


#include "Architecture/Door.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	}
	if (!DoorFrame)
	{
		DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh>FrameMeshRef(TEXT("/Game/Blueprints/Architecture/Door/Mesh/SM_Door_06b"));
		if (FrameMeshRef.Succeeded())
		{
			DoorFrame->SetStaticMesh(FrameMeshRef.Object);
		}

		DoorFrame->SetupAttachment(RootComponent);
	}
	if (!Door)
	{
		Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh>DoorMeshRef(TEXT("/Game/Blueprints/Architecture/Door/Mesh/SM_Door_06a"));
		if (DoorMeshRef.Succeeded())
		{
			Door->SetStaticMesh(DoorMeshRef.Object);
		}

		Door->SetupAttachment(DoorFrame);
	}
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpen)
	{
		if (!Door)
			return;

		if (OpenDirection > 0)
		{
			if (Door->GetRelativeRotation().Yaw > -90)
			{
				Door->AddRelativeRotation(FRotator(0, -DeltaRotate * DeltaTime, 0));
			}
		}
		else
		{
			if (Door->GetRelativeRotation().Yaw < 90)
			{
				Door->AddRelativeRotation(FRotator(0, DeltaRotate * DeltaTime, 0));
			}
		}
	}
	else
	{
		if (!Door)
			return;
		if (Door->GetRelativeRotation().Yaw > 0.5f)
		{
			Door->AddRelativeRotation(FRotator(0, -DeltaRotate * DeltaTime, 0));
		}
		else if (Door->GetRelativeRotation().Yaw < -0.5f)
		{
			Door->AddRelativeRotation(FRotator(0, DeltaRotate * DeltaTime, 0));
		}
	}
}

void ADoor::Open(FVector _ActorLocation)
{
	if (IsOpen)
	{
		IsOpen = false;
	}
	else
	{
		FVector DoorToActor = (_ActorLocation - GetActorLocation()) * FVector(1, 1, 0);
		FVector DoorVector = GetActorForwardVector() * FVector(1, 1, 0);

		OpenDirection = FVector::DotProduct(DoorToActor, DoorVector);

		IsOpen = true;
	}
}
