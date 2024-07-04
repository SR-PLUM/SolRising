// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Item/Item.h"
#include "UI/ItemWidget.h"
#include "UI/GunSlot.h"
#include "Struct/ItemData.h"

UInventory::UInventory(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ItemWidgetBPClass(TEXT("/Game/Blueprints/UI/WBP_ItemWidget"));
	if (ItemWidgetBPClass.Class != nullptr)
	{
		ItemWidgetClass = ItemWidgetBPClass.Class;
	}
}

void UInventory::ClearPickList()
{
	if (PickableItemList)
	{
		auto childrenList = PickableItemList->GetAllChildren();
		for(int i = childrenList.Num() - 1; i >= 0; i--)
		{
			PickableItemList->RemoveChild(childrenList[i]);
		}
	}
}

void UInventory::AddList(AItem* item)
{
	if (!ItemWidgetClass) return;
	if (!item) return;

	UItemWidget* itemWidget = CreateWidget<UItemWidget>(this, ItemWidgetClass);
	if (itemWidget && PickableItemList)
	{
		itemWidget->Item = item;
		itemWidget->Owner = Owner;
		itemWidget->isPickable = true;

		if(itemWidget->ItemText)
			itemWidget->ItemText->SetText(item->itemName);
		if (itemWidget->ItemImg)
			itemWidget->ItemImg->SetBrushFromTexture(item->itemImg);
		if (itemWidget->ItemCntText)
		{
			if(item->count == 0)
				itemWidget->ItemCntText->SetText(FText::FromString(" "));
			else
				itemWidget->ItemCntText->SetText(FText::FromString(FString::FromInt(item->count)));
		}

		PickableItemList->AddChild(itemWidget);

		ItemList.Add(item, itemWidget);
	}
}

void UInventory::RemoveList(AItem* item)
{
	if (!ItemWidgetClass) return;
	if (!item) return;

	auto removedWidget = ItemList.FindAndRemoveChecked(item);
	if (removedWidget)
	{
		PickableItemList->RemoveChild(removedWidget);
	}
}

void UInventory::AddInventory(FItemData itemData, UTexture2D* itemImg)
{
	UItemWidget* itemWidget = CreateWidget<UItemWidget>(this, ItemWidgetClass);
	if (itemWidget && PickedItemList)
	{
		itemWidget->isPickable = false;
		itemWidget->ItemName = itemData.itemName.ToString();
		itemWidget->Owner = Owner;

		if (itemWidget->ItemText)
			itemWidget->ItemText->SetText(itemData.itemName);
		if (itemWidget->ItemImg)
			itemWidget->ItemImg->SetBrushFromTexture(itemImg);
		if (itemWidget->ItemCntText)
			itemWidget->ItemCntText->SetText(FText::FromString(FString::FromInt(itemData.count)));

		PickedItemList->AddChild(itemWidget);
		HavingItemList.Add(itemData.itemName.ToString(),itemWidget);
	}
}

void UInventory::RemoveInventory(FItemData itemData)
{
	auto removedWidget = HavingItemList.FindAndRemoveChecked(itemData.itemName.ToString());
	if (removedWidget)
	{
		PickedItemList->RemoveChild(removedWidget);
	}
}

void UInventory::RefreshMainGunSlot(AGun* gun)
{
	MainGunSlot->RefreshGunSlot(gun);
}

void UInventory::RefreshSubGunSlot(AGun* gun)
{
	SubGunSlot->RefreshGunSlot(gun);
}

void UInventory::RefreshPickableItemCount(AItem* Item, int32 cnt)
{
	auto updateItem = ItemList.FindRef(Item);
	if (updateItem)
	{
		updateItem->ItemCntText->SetText(FText::FromString(FString::FromInt(cnt)));
	}
}

void UInventory::RefreshHavingItemCount(const FString& itemName, int32 cnt)
{
	auto updateItem = HavingItemList.FindRef(itemName);
	if (updateItem)
	{
		updateItem->ItemCntText->SetText(FText::FromString(FString::FromInt(cnt)));
	}
}
