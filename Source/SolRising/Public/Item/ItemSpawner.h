// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

UENUM(BlueprintType)
enum class E_ItemType : uint8
{
	EIT_Ammo UMETA(DisplayName = "Ammo"),
	EIT_Bag UMETA(DisplayName = "Bag"),
	EIT_Gun UMETA(DisplayName = "Gun"),
};

UCLASS()
class SOLRISING_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	class AAmmo* SpawnAmmo(FVector Location);
	UFUNCTION()
	void SpawnBag();
	UFUNCTION()
	void SpawnGun();

private:
	const int NumberOfItemType = 3;

	TSubclassOf<class AAmmo> AmmoBP;
	TSubclassOf<class ABag> BagBP;
	TSubclassOf<class AGun> GunBP;
};
