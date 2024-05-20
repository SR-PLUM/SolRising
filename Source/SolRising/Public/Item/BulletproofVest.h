// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "BulletproofVest.generated.h"

UENUM(BlueprintType)
enum class E_BulletproofVestType : uint8
{
	EBVT_LV1 UMETA(DisplayName = "LV1 Vest"),
	EBVT_LV2 UMETA(DisplayName = "LV2 Vest"),
	EBVT_LV3 UMETA(DisplayName = "LV3 Vest")
};

UCLASS()
class SOLRISING_API ABulletproofVest : public AItem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABulletproofVest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float defense;

public:
	float GetDefence();

private:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BulletproofVestMesh;
	UPROPERTY(EditAnywhere)
	class UStaticMesh* Lv1VestMesh;
	UPROPERTY(EditAnywhere)
	class UStaticMesh* Lv2VestMesh;
	UPROPERTY(EditAnywhere)
	class UStaticMesh* Lv3VestMesh;
};
