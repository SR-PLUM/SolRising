// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GunSlot.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Item/Gun.h"

void UGunSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GunImg)
	{
		GunImg->SetVisibility(ESlateVisibility::Hidden);
	}

	FString AK47Path("/Game/Blueprints/UI/AK_47");
	FString AK74UPath("/Game/Blueprints/UI/AK_74U");
	FString M416Path("/Game/Blueprints/UI/M_416");

	AK47Img = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *AK47Path));
	AK74UImg = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *AK74UPath));
	M416Img = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *M416Path));
}

void UGunSlot::RefreshGunSlot(AGun* gun)
{
	if (!gun) return;

	GunImg->SetVisibility(ESlateVisibility::Visible);

	switch (gun->currentGunName)
	{
	case E_GunName::EGN_AK47:
		GunImg->SetBrushFromTexture(AK47Img);
		break;
	case E_GunName::EGN_AK74U:
		GunImg->SetBrushFromTexture(AK74UImg);
		break;
	case E_GunName::EGN_M416:
		GunImg->SetBrushFromTexture(M416Img);
		break;
	default:
		break;
	}

	GunName->SetText(gun->GetGunName());

	Ammo->SetText(FText::FromString(FString::FromInt(gun->GetLoadedAmmo())));
}
