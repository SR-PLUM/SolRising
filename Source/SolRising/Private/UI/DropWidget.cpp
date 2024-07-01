// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DropWidget.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Character/Solaris.h"

void UDropWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (InputBox)
	{
		InputBox->OnTextChanged.AddDynamic(this, &UDropWidget::OnChangedText);
	}
	if (CntSlider)
	{
		CntSlider->OnValueChanged.AddDynamic(this, &UDropWidget::OnChangedValue);
	}
	if (DropConfirmBtn)
	{
		DropConfirmBtn->OnClicked.AddDynamic(this, &UDropWidget::Confirm);
	}
}

void UDropWidget::OnChangedText(const FText& text)
{
	auto number = FCString::Atoi(*text.ToString());
	CntSlider->SetValue(number);
}

void UDropWidget::OnChangedValue(float value)
{
	InputBox->SetText(FText::FromString(FString::FromInt(value)));
}

void UDropWidget::SetDropItem(FItemData Item)
{
	currentItem = Item;
	HavingItemCnt->SetText(FText::FromString(FString::FromInt(Item.count)));
	CntSlider->SetMaxValue(Item.count);
	CntSlider->SetMinValue(1);
	CntSlider->SetValue(1);
}

void UDropWidget::Confirm()
{
	if (Owner && CntSlider)
	{
		Owner->Drop(currentItem, CntSlider->GetValue());
	}

	SetVisibility(ESlateVisibility::Hidden);
}
