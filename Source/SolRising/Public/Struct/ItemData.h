// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class E_ItemType : uint8
{
	EIT_Ammo UMETA(DisplayName = "Ammo"),
	EIT_HealItem UMETA(DisplayName = "HealItem"),
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY()
	E_ItemType itemType;
	UPROPERTY()
	FText itemName;
	UPROPERTY()
	int32 subType;
	UPROPERTY()
	int32 count;
	UPROPERTY()
	float individualWeight;

	bool operator== (FItemData data)
	{
		if (itemName.ToString() == data.itemName.ToString())
			return true;
		else
			return false;
	}
};
