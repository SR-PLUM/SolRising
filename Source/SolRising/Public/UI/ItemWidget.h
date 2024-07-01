// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOLRISING_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemCntText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemText;

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImg;

	UPROPERTY(meta = (BindWidget))
	class UButton* ItemPickBtn;

	UPROPERTY()
	class AItem* Item;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString ItemName;

	UFUNCTION()
	void PickItem();

	class ASolaris* Owner;

	UPROPERTY()
	bool isPickable = true;
};
