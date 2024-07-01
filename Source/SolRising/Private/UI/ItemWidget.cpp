// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemWidget.h"

#include "Components/Button.h"

#include "Character/Solaris.h"

void UItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ItemPickBtn)
	{
		ItemPickBtn->OnClicked.AddDynamic(this, &UItemWidget::PickItem);
	}

}

void UItemWidget::PickItem()
{
	if(Owner && isPickable)
		Owner->Pick(Item);
	if (Owner && !isPickable)
	{
		Owner->ShowDropWidget(ItemName);
	}
}
