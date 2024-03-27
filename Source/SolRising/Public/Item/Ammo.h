// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Ammo.generated.h"

UENUM(BlueprintType)
enum class E_AmmoType : uint8
{
	EAT_5 UMETA(DisplayName = "5.56mm Ammo"),
	EAT_7 UMETA(DisplayName = "7.62mm Ammo"),
};

UCLASS()
class SOLRISING_API AAmmo : public AItem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* AmmoMesh;

public:
	int32 count;
	int32 ammoType_gen;
};
