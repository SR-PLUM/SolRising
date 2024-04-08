// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class SOLRISING_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorFrame;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Door;
	UPROPERTY()
	bool IsOpen = false;
	UPROPERTY()
	float DeltaRotate = 100;
	UPROPERTY()
	float OpenDirection = 0.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Open(FVector ActorLocation);
};
