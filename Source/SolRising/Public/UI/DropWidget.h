// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/ItemData.h"
#include "DropWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOLRISING_API UDropWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HavingItemCnt;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* InputBox;

	UPROPERTY(meta = (BindWidget))
	class USlider* CntSlider;

	UPROPERTY(meta = (BindWidget))
	class UButton* DropConfirmBtn;

	UFUNCTION()
	void OnChangedText(const FText& text);
	UFUNCTION()
	void OnChangedValue(float value);

	UFUNCTION()
	void SetDropItem(FItemData Item);

	UFUNCTION()
	void Confirm();

	UPROPERTY()
	class ASolaris* Owner;
	UPROPERTY()
	FItemData currentItem;
};
