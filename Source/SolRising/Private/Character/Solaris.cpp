// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Solaris.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"

#include "Item/Item.h"
#include "Item/Gun.h"
#include "Item/Bag.h"
#include "Item/BulletproofVest.h"
#include "Item/Ammo.h"
#include "Item/HealItem.h"
#include "Architecture/Door.h"
#include "UI/Inventory.h"
#include "Struct/ItemData.h"
#include "UI/DropWidget.h"

ASolaris::ASolaris()
{
 	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	FPCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("FPCameraBoom"));
	FPCameraBoom->SetupAttachment(GetMesh());
	FPCameraBoom->TargetArmLength = 10.f;

	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FPCamera->SetupAttachment(FPCameraBoom);

	PickItemRange = CreateDefaultSubobject<USphereComponent>(TEXT("PickItemRange"));
	PickItemRange->SetupAttachment(RootComponent);

	currentHP = 50.f;

	ConstructorHelpers::FClassFinder<UUserWidget> InventoryBPClass(TEXT("/Game/Blueprints/UI/WBP_Inventory"));
	if (InventoryBPClass.Class != nullptr)
	{
		InventoryWidgetClass = InventoryBPClass.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> DropBPClass(TEXT("/Game/Blueprints/UI/WBP_DropWidget"));
	if (DropBPClass.Class != nullptr)
	{
		DropWidgetClass = DropBPClass.Class;
	}

	static ConstructorHelpers::FObjectFinder<UClass> AmmoRef(TEXT("Blueprint'/Game/Blueprints/Item/BP_Ammo.BP_Ammo_C'"));
	if (AmmoRef.Object)
	{
		AmmoBP = (UClass*)AmmoRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UClass> HealItemRef(TEXT("Blueprint'/Game/Blueprints/Item/BP_HealItem.BP_HealItem_C'"));
	if (HealItemRef.Object)
	{
		HealItemBP = (UClass*)HealItemRef.Object;
	}
}

void ASolaris::BeginPlay()
{
	Super::BeginPlay();	
	
	PickItemRange->OnComponentBeginOverlap.AddDynamic(this, &ASolaris::OnItemBeginOverlap);
	PickItemRange->OnComponentEndOverlap.AddDynamic(this, &ASolaris::OnItemEndOverlap);
}

void ASolaris::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASolaris::MoveForward(float Value)
{
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void ASolaris::MoveRight(float Value)
{
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void ASolaris::Turn(float Value)
{
	AddControllerYawInput(Value);
	//UE_LOG(LogTemp,Warning,TEXT("%f"),GetController()->GetControlRotation().Yaw)
}

void ASolaris::LookUp(float Value)
{
	AddControllerPitchInput(Value);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), GetController()->GetControlRotation().Pitch)
}

void ASolaris::Interaction()
{
	AItem* pickedItem = nullptr;

	FHitResult CameraHit;
	auto bIsCameraHit = LineTracingMouse(CameraHit);

	//Interaction Using Mouse Cursor
	if (bIsCameraHit)
	{
		//Door Action
		auto CameraHitDoor = Cast<ADoor>(CameraHit.GetActor());
		if (CameraHitDoor)
		{
			if (GetDistanceTo(CameraHitDoor) <= 400.f)
			{
				CameraHitDoor->Open(GetActorLocation());
				return;
			}
		}

		//Pick Action Using Mouse Cursor
		auto CameraHitItem = Cast<AItem>(CameraHit.GetActor());
		if (CameraHitItem)
		{
			pickedItem = CameraHitItem;
		}
	}

	Pick(pickedItem);
}

void ASolaris::Pick(AItem* pickedItem)
{
	//If Can't Detect Any Item at Mouse Cursor
	//Check Item Near by Solaris
	if (!pickedItem)
	{
		if (OverlappedItem.IsEmpty())
			return;

		pickedItem = OverlappedItem[0];
		//OverlappedItem.Remove(pickedItem);
	}

	//If PickedItem is Gun
	AGun* gun = Cast<AGun>(pickedItem);
	if (gun)
	{
		if (MainGun == nullptr)
		{
			SetMainGun(gun);

			CurrentGun = MainGun;
		}
		else if (SubGun == nullptr)
		{
			SetSubGun(gun);

			UE_LOG(LogTemp, Warning, TEXT("서브건 장착"));
		}
		else
		{
			// TODO 
			// 메인건에 장전되어 있는 총알 수거
			if (GetMainGun()->GetLoadedAmmo() > 0)
			{
				if (CanPick(GetMainGun()->GetAmmoWeight()))
				{
					CurrentWeight += GetMainGun()->GetAmmoWeight();
					currentAmmoCount[GetMainGun()->GetAmmoType()] += GetMainGun()->GetLoadedAmmo();
				}
			}

			MainGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			UE_LOG(LogTemp, Warning, TEXT("메인건 버리기"));
			
			SetMainGun(gun);

			MainGun->AttachMeshToSocket(GetMesh(), FName("RightHandIdleSocket"));
		}

		OverlappedItem.Remove(pickedItem);
		return;
	}

	//If PickedItem is Bag
	ABag* bag = Cast<ABag>(pickedItem);
	if (bag)
	{
		if (MaxWeight <= bag->GetMaxWeight())
		{
			Bag = bag;
			MaxWeight = bag->GetMaxWeight();
			Bag->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));

			UE_LOG(LogTemp, Warning, TEXT("새로 먹은 가방 최대 용량 : %f, 현재 가방용량 : %f"), Bag->GetMaxWeight(), CurrentWeight);

			OverlappedItem.Remove(pickedItem);
		}

		return;
	}

	//If PickedItem is BulletproofVest
	ABulletproofVest* vest = Cast<ABulletproofVest>(pickedItem);
	if (vest)
	{
		CurrentVestDefence = vest->GetDefence();

		UE_LOG(LogTemp, Warning, TEXT("조끼 방어력 : %f"), vest->GetDefence());

		vest->Destroy();					//Destroy 하지 않기
		OverlappedItem.Remove(vest);

		return;
	}

	//If PickedItem is Ammo
	AAmmo* ammo = Cast<AAmmo>(pickedItem);
	if (ammo)
	{
		if (CanPick(ammo->weight))
		{
			CurrentWeight += ammo->weight;
			bool hasItem = false;

			for (auto& item : havingItems)
			{
				if (item.itemType == E_ItemType::EIT_Ammo)
				{
					if (item.subType == ammo->ammoType)
					{
						item.count += ammo->count;
						hasItem = true;

						if (InventoryWidget)
						{
							InventoryWidget->RefreshHavingItemCount(item.itemName.ToString(), item.count);
						}

						break;
					}
				}
			}

			if (hasItem == false)
			{
				FItemData tmpItem;
				tmpItem.count = pickedItem->count;
				tmpItem.individualWeight = pickedItem->individualWeight;
				tmpItem.itemType = E_ItemType::EIT_Ammo;
				tmpItem.subType = ammo->ammoType;
				tmpItem.itemName = ammo->itemName;
				havingItems.Add(tmpItem);

				if (InventoryWidget)
				{
					InventoryWidget->AddInventory(tmpItem, ammo->itemImg);
				}
			}

			ammo->Destroy();
			OverlappedItem.Remove(ammo);
		}
		else
		{
			PartialPick(ammo);
		}

		return;
	}

	AHealItem* healItem = Cast<AHealItem>(pickedItem);
	if(healItem)
	{
		if (CanPick(healItem->weight))
		{
			CurrentWeight += healItem->weight;

			bool hasItem = false;

			for (auto& item : havingItems)
			{
				if (item.itemType == E_ItemType::EIT_HealItem)
				{
					if (item.subType == healItem->healItemType)
					{
						item.count += healItem->count;
						hasItem = true;

						if (InventoryWidget)
						{
							InventoryWidget->RefreshHavingItemCount(item.itemName.ToString(), item.count);
						}

						break;
					}
				}
			}

			if (hasItem == false)
			{
				FItemData tmpItem;
				tmpItem.count = pickedItem->count;
				tmpItem.individualWeight = pickedItem->individualWeight;
				tmpItem.itemType = E_ItemType::EIT_HealItem;
				tmpItem.subType = healItem->healItemType;
				tmpItem.itemName = healItem->itemName;
				havingItems.Add(tmpItem);

				if (InventoryWidget)
				{
					InventoryWidget->AddInventory(tmpItem, healItem->itemImg);
				}
			}

			healItem->Destroy();
			OverlappedItem.Remove(healItem);
		}
		else
		{
			PartialPick(healItem);
		}

		return;
	}
}

void ASolaris::ShowDropWidget(FString dropedItem)
{
	if (!DropWidget)
	{
		if (DropWidgetClass)
		{
			DropWidget = Cast<UDropWidget>(CreateWidget(GetWorld(), DropWidgetClass));

			if (DropWidget)
			{
				DropWidget->Owner = this;
				DropWidget->AddToViewport();
				DropWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	if (DropWidget)
	{
		for (auto item : havingItems)
		{
			if (item.itemName.ToString() == dropedItem)
			{
				if (item.count == 1)
				{
					Drop(item, 1);
					return;
				}
				DropWidget->SetDropItem(item);
				break;
			}
		}
		DropWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ASolaris::Drop(FItemData dropItem, int32 cnt)
{
	bool IsDeleted = false;
	for (auto& item : havingItems)
	{
		if (item.itemName.EqualTo(dropItem.itemName))
		{
			if (cnt == item.count)
			{
				// TODO
				// 배열에서 아이템 제거하기
				IsDeleted = true;
				// UI에서 위젯 제거하기
				InventoryWidget->RemoveInventory(item);
			}
			else
			{
				// TODO
				// 배열의 아이템 갯수 줄이기
				item.count -= cnt;
				// 위젯 아이템 갯수 새로고침
				InventoryWidget->RefreshHavingItemCount(item.itemName.ToString(), item.count);
			}

			//발밑에 아이템 뿌리기
			if (item.itemType == E_ItemType::EIT_Ammo)
			{
				auto World = GetWorld();
				if (World)
				{
					FActorSpawnParameters SpawnParameter;
					SpawnParameter.Owner = this;
					SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					FTransform SpawnTransform;
					SpawnTransform.SetLocation(GetActorLocation() - FVector(0, 0, 90.f));
					SpawnTransform.SetRotation(GetActorQuat());

					auto SpawnedAmmo = World->SpawnActor<AAmmo>(AmmoBP, SpawnTransform, SpawnParameter);
					if (SpawnedAmmo)
					{
						OverlappedItem.Remove(SpawnedAmmo);

						if (InventoryWidget)
						{
							InventoryWidget->RemoveList(SpawnedAmmo);
						}

						SpawnedAmmo->count = cnt;

						if (item.subType == (int32)E_AmmoType::EAT_5)
						{
							SpawnedAmmo->ChangeAmmoType(E_AmmoType::EAT_5);
						}
						if (item.subType == (int32)E_AmmoType::EAT_7)
						{
							SpawnedAmmo->ChangeAmmoType(E_AmmoType::EAT_7);
						}

						CurrentWeight -= SpawnedAmmo->weight;

						OverlappedItem.Add(SpawnedAmmo);
						if (InventoryWidget)
						{
							InventoryWidget->AddList(SpawnedAmmo);
						}
					}
				}
			}
			if (item.itemType == E_ItemType::EIT_HealItem)
			{
				auto World = GetWorld();
				if (World)
				{
					FActorSpawnParameters SpawnParameter;
					SpawnParameter.Owner = this;
					SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					FTransform SpawnTransform;
					SpawnTransform.SetLocation(GetActorLocation() - FVector(0, 0, 90.f));
					SpawnTransform.SetRotation(GetActorQuat());

					auto SpawnedHealItem = World->SpawnActor<AHealItem>(HealItemBP, SpawnTransform, SpawnParameter);
					if (SpawnedHealItem)
					{
						OverlappedItem.Remove(SpawnedHealItem);

						if (InventoryWidget)
						{
							InventoryWidget->RemoveList(SpawnedHealItem);
						}

						SpawnedHealItem->count = cnt;

						if (item.subType == (int32)E_HealItemType::EHT_FirstAidKit)
						{
							SpawnedHealItem->ChangeHealItemType(E_HealItemType::EHT_FirstAidKit);
						}
						if (item.subType == (int32)E_HealItemType::EHT_Bandage)
						{
							SpawnedHealItem->ChangeHealItemType(E_HealItemType::EHT_Bandage);
						}

						CurrentWeight -= SpawnedHealItem->weight;

						OverlappedItem.Add(SpawnedHealItem);
						if (InventoryWidget)
						{
							InventoryWidget->AddList(SpawnedHealItem);
						}
					}
				}
			}

			break;
		}
	}

	if (IsDeleted)
	{
		havingItems.Remove(dropItem);
	}
}

void ASolaris::Kneel()
{
	if (GroundPose == E_GroundPose::EGP_Kneel)
	{
		GroundPose = E_GroundPose::EGP_Standing;
	}
	else
	{
		GroundPose = E_GroundPose::EGP_Kneel;
	}
}

void ASolaris::Prone()
{
	if (GroundPose == E_GroundPose::EGP_Prone)
	{
		GroundPose = E_GroundPose::EGP_Standing;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	else
	{
		GroundPose = E_GroundPose::EGP_Prone;
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
}

bool ASolaris::Inventory()
{
	if (!InventoryWidget)
	{
		if (InventoryWidgetClass)
		{
			InventoryWidget = Cast<UInventory>(CreateWidget(GetWorld(), InventoryWidgetClass));
			if (InventoryWidget)
			{
				InventoryWidget->Owner = this;

				for (auto item : OverlappedItem)
				{
					InventoryWidget->AddList(item);
				}

				InventoryWidget->AddToViewport();
				IsInventoryOpen = true;

				return true;
			}
		}

		return false;
	}

	if (IsInventoryOpen)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		IsInventoryOpen = false;

		return false;
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		IsInventoryOpen = true;

		return true;
	}
}

bool ASolaris::CanPick(float itemWeight)
{
	if (itemWeight <= MaxWeight - CurrentWeight)
		return true;
	else
		return false;
}

void ASolaris::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AItem* item = Cast<AItem>(OtherActor))
	{
		OverlappedItem.Add(item);

		if (InventoryWidget)
		{
			InventoryWidget->AddList(item);
		}
	}
}

void ASolaris::PartialPick(AItem* item)
{	
	if (AGun* gun = Cast<AGun>(item))
		return;

	if (ABulletproofVest* vest = Cast<ABulletproofVest>(item))
		return;

	if (ABag* bag = Cast<ABag>(item))
		return;

	int32 canPickNumber = DecideCount(item);

	if (canPickNumber > 0)
	{
		CurrentWeight += item->individualWeight * canPickNumber;

		AHealItem* healItem = Cast<AHealItem>(item);
		if (healItem)
		{
			bool hasItem = false;

			for (auto& havingItem : havingItems)
			{
				if (havingItem.itemType == E_ItemType::EIT_HealItem)
				{
					if (havingItem.subType == healItem->healItemType)
					{
						havingItem.count += canPickNumber;
						hasItem = true;

						if (InventoryWidget)
						{
							InventoryWidget->RefreshHavingItemCount(havingItem.itemName.ToString(), havingItem.count);
						}

						break;
					}
				}
			}

			if (hasItem == false)
			{
				FItemData tmpItem;
				tmpItem.count = canPickNumber;
				tmpItem.individualWeight = item->individualWeight;
				tmpItem.itemType = E_ItemType::EIT_HealItem;
				tmpItem.subType = healItem->healItemType;
				tmpItem.itemName = healItem->itemName;
				havingItems.Add(tmpItem);

				if (InventoryWidget)
				{
					InventoryWidget->AddInventory(tmpItem, item->itemImg);
				}
			}
		}

		AAmmo* ammo = Cast<AAmmo>(item);
		if (ammo)
		{
			bool hasItem = false;

			for (auto& havingItem : havingItems)
			{
				if (havingItem.itemType == E_ItemType::EIT_Ammo)
				{
					if (havingItem.subType == ammo->ammoType)
					{
						havingItem.count += canPickNumber;
						hasItem = true;

						if (InventoryWidget)
						{
							InventoryWidget->RefreshHavingItemCount(havingItem.itemName.ToString(), havingItem.count);
						}

						break;
					}
				}
			}

			if (hasItem == false)
			{
				FItemData tmpItem;
				tmpItem.count = canPickNumber;
				tmpItem.individualWeight = item->individualWeight;
				tmpItem.itemType = E_ItemType::EIT_Ammo;
				tmpItem.subType = ammo->ammoType;
				tmpItem.itemName = ammo->itemName;
				havingItems.Add(tmpItem);

				if (InventoryWidget)
				{
					InventoryWidget->AddInventory(tmpItem, item->itemImg);
				}
			}
		}

		item->count -= canPickNumber;
		item->weight = item->individualWeight * item->count;
		if (InventoryWidget)
		{
			InventoryWidget->RefreshPickableItemCount(item, item->count);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("획득 실패"));
	}

	return;
}

int32 ASolaris::DecideCount(AItem* item)
{
	return ((MaxWeight - CurrentWeight) / item->individualWeight);
}

void ASolaris::OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AItem* item = Cast<AItem>(OtherActor))
	{
		OverlappedItem.Remove(item);

		if (InventoryWidget)
		{
			InventoryWidget->RemoveList(item);
		}
	}
}

void ASolaris::SetMainGun(AGun* gun)
{
	MainGun = gun;

	MainGun->AttachMeshToSocket(GetMesh(), FName("RightHandIdleSocket"));
	MainGun->SetOwningCharacter(this);

	if (InventoryWidget)
	{
		InventoryWidget->RefreshMainGunSlot(MainGun);
	}

	UE_LOG(LogTemp, Warning, TEXT("메인건 장착"));
}

AGun* ASolaris::GetMainGun()
{
	if (MainGun)
		return MainGun;

	return nullptr;
}

void ASolaris::SetSubGun(AGun* gun)
{
	SubGun = gun;

	gun->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));

	if (InventoryWidget)
	{
		InventoryWidget->RefreshSubGunSlot(SubGun);
	}
}

AGun* ASolaris::GetSubGun()
{
	if (SubGun)
		return SubGun;

	return nullptr;
}

FVector ASolaris::GetCameraLocation()
{
	if (!ViewCamera)
		return FVector();

	return ViewCamera->GetComponentLocation();
}

FRotator ASolaris::GetCameraRotation()
{
	if(!ViewCamera || !FPCamera)
		return FRotator();

	if (ViewCamera->IsActive())
		return ViewCamera->GetComponentRotation();
	else if (FPCamera->IsActive())
		return FPCamera->GetComponentRotation();
	else
		return FRotator();
}

bool ASolaris::LineTracingMouse(FHitResult& CameraHit)
{
	const float TraceDistance = 500.f;
	UWorld* World = GetWorld();

	if (World)
	{
		FVector StartCameraTrace;
		if (ViewCamera && FPCamera)
		{
			if (IsAiming == false)
			{
				StartCameraTrace = ViewCamera->GetComponentLocation();
			}
			else if (IsAiming == true)
			{
				StartCameraTrace = FPCamera->GetComponentLocation();
			}
		}
		
		FVector EndCameraTrace = StartCameraTrace + (GetCameraRotation().Vector() * TraceDistance);
		ECollisionChannel ECC_CameraHit = ECC_Visibility;
		bool bIsHitCamera = World->LineTraceSingleByChannel(CameraHit, StartCameraTrace, EndCameraTrace, ECC_CameraHit);
		if (bIsHitCamera)
		{
			DrawDebugLine(World, StartCameraTrace, EndCameraTrace, FColor::Red, false, 2.f);
			return true;
		}
	}

	return false;
}

void ASolaris::TogglePerspective()
{
	if (IsAiming == false)
	{
		FPCamera->Activate();
		ViewCamera->Deactivate();
	}
	else if (IsAiming == true)
	{
		ViewCamera->Activate();
		FPCamera->Deactivate();
	}
}

void ASolaris::Aiming()
{
	if (IsAiming == false)
	{
		FPCamera->Activate();
		ViewCamera->Deactivate();

		IsAiming = true;
	}
	else if (IsAiming == true)
	{
		ViewCamera->Activate();
		FPCamera->Deactivate();

		IsAiming = false;
	}
}

void ASolaris::PlayFireMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayFireMonatge"));
		AnimInstance->Montage_Play(FireMontage);
		AnimInstance->Montage_JumpToSection("IdleFire", FireMontage);
	}
}

void ASolaris::RefreshInventory()
{

}

float ASolaris::GetCurrentVestDefence()
{
	return CurrentVestDefence;
}

void ASolaris::TakeDamege(bool isHeadShot, float damage, float reduceDamage)
{
	if (isHeadShot == true)
	{
		currentHP -= damage * 2;
	}
	else
	{
		currentHP -= damage * reduceDamage;
	}
}

void ASolaris::HealHP(float AmountOfRecovery)
{
	if (currentHP + AmountOfRecovery > maxHP)
	{
		currentHP = maxHP;
	}
	else
	{
		currentHP += AmountOfRecovery;
	}	
}

void ASolaris::UseItem(FItemData itemData)
{
	if (itemData.itemType == E_ItemType::EIT_HealItem)
	{
		if (itemData.subType == (int)E_HealItemType::EHT_FirstAidKit)
		{
			HealHP(AHealItem::firstAidKitRecovery);
		}
		else if (itemData.subType == (int)E_HealItemType::EHT_Bandage)
		{
			HealHP(AHealItem::bandageRecovery);
		}

		itemData.count -= 1;
		CurrentWeight -= itemData.individualWeight;
	}
}

float ASolaris::GetHP()
{
	return currentHP;
}

float ASolaris::GetCurrentGunDamage()
{
	if (CurrentGun)
		return CurrentGun->damage;
	else 
		return 0.f;
}

void ASolaris::AttachMainGun()
{
	if (SubGun)
	{
		SubGun->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
	MainGun->AttachMeshToSocket(GetMesh(), FName("RightHandIdleSocket"));
}

void ASolaris::AttachSubGun()
{
	if (MainGun)
	{
		MainGun->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
	SubGun->AttachMeshToSocket(GetMesh(), FName("RightHandIdleSocket"));
}

