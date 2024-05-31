// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "HealItem.generated.h"

UENUM(BlueprintType)
enum class E_HelaItemType : uint8
{
	EHT_FirstAidKit UMETA(DisplayName = "FirstAidKit"),
	EHT_Bandage UMETA(DisplayName = "Bandage"),
};

UCLASS()
class SOLRISING_API AHealItem : public AItem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AHealItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* HealItemMesh;
	UPROPERTY(EditAnywhere)
	class UStaticMesh* FirstAidKitMesh;
	UPROPERTY(EditAnywhere)
	class UStaticMesh* BandageMesh;

	UPROPERTY()
	UTexture2D* FirstAidKitImg;
	UPROPERTY()
	UTexture2D* BandageImg;

private:
	float amountOfRecovery;

public:
	int32 healItemType;
	float GetAmountOfRecovery();
};
