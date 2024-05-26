// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "Item/Item.h"
#include "UI/ItemWidget.h"
#include "UI/GunSlot.h"

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

void UInventory::AddInventory(AItem* item)
{
	UItemWidget* itemWidget = CreateWidget<UItemWidget>(this, ItemWidgetClass);
	if (itemWidget && PickedItemList)
	{
		itemWidget->Item = item;
		itemWidget->Owner = Owner;
		itemWidget->isPickable = false;

		if (itemWidget->ItemText)
			itemWidget->ItemText->SetText(item->itemName);

		PickedItemList->AddChild(itemWidget);
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
