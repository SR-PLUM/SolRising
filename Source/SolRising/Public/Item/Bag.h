// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Bag.generated.h"

UENUM(BlueprintType)
enum class E_BagType : uint8
{
	EBT_SmallBag UMETA(DisplayName = "SmallBag"),
	EBT_MediumBag UMETA(DisplayName = "MediumBag"),
	EBT_LargeBag UMETA(DisplayName = "LargeBag")
};

UCLASS()
class SOLRISING_API ABag : public AItem
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ABag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float maxWeight;

public:
	float GetMaxWeight();
	
private:
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* BagMesh;
	UPROPERTY(EditAnywhere)
		class UStaticMesh* Lv1BagMesh;
	UPROPERTY(EditAnywhere)
		class UStaticMesh* Lv2BagMesh;
	UPROPERTY(EditAnywhere)
		class UStaticMesh* Lv3BagMesh;
};
