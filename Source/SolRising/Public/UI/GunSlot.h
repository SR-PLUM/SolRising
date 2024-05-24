// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GunSlot.generated.h"

/**
 * 
 */
UCLASS()
class SOLRISING_API UGunSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* AK74UImg;
	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* AK47Img;
	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* M416Img;

	UPROPERTY(meta = (BindWidget))
	class UImage* GunImg;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GunName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Ammo;

	UFUNCTION()
	void RefreshGunSlot(class AGun* gun);

};
