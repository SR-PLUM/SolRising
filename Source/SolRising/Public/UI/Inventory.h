// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class SOLRISING_API UInventory : public UUserWidget
{
	GENERATED_BODY()
public:
	UInventory(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* PickableItemList;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* PickedItemList;

	UPROPERTY(meta = (BindWidget))
	class UGunSlot* MainGunSlot;
	UPROPERTY(meta = (BindWidget))
	class UGunSlot* SubGunSlot;

	UPROPERTY()
	TSubclassOf<class UUserWidget> ItemWidgetClass;

	TMap<class AItem*, class UItemWidget*> ItemList;

public:
	UFUNCTION()
	void ClearPickList();

	UFUNCTION()
	void AddList(class AItem* item);
	UFUNCTION()
	void RemoveList(class AItem* item);

	UFUNCTION()
	void AddInventory(class AItem* item);

	UFUNCTION()
	void RefreshMainGunSlot(class AGun* gun);
	UFUNCTION()
	void RefreshSubGunSlot(class AGun* gun);

	class ASolaris* Owner;
};
