// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Solaris.generated.h"

UENUM(BlueprintType)
enum class E_GroundPose : uint8
{
	EGP_Standing UMETA(DisplayName = "Standing"),
	EGP_Kneel UMETA(DisplayName = "Kneel"),
	EGP_Prone UMETA(DisplayName = "Prone"),
};

UCLASS()
class SOLRISING_API ASolaris : public ACharacter
{
	GENERATED_BODY()

public:
	ASolaris();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION()
	void MoveForward(float Value);
	UFUNCTION()
	void MoveRight(float Value);
	UFUNCTION()
	void Turn(float Value);
	UFUNCTION()
	void LookUp(float Value);

	UFUNCTION()
	void Interaction();
	UFUNCTION()
	void Pick(AItem* pickedItem);
	UFUNCTION()
	void Kneel();
	UFUNCTION()
	void Prone();
	UFUNCTION()
	bool Inventory();

	UFUNCTION()
	bool CanPick(float ItemWeight);

private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* ViewCamera;
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* FPCameraBoom;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FPCamera;
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* PickItemRange;

	UPROPERTY()
	TArray<class AItem*> OverlappedItem;

	UPROPERTY(VisibleAnywhere)
	class AGun* MainGun = nullptr;
	UPROPERTY(VisibleAnywhere)
	class AGun* SubGun = nullptr;
	UPROPERTY(VisibleAnywhere)
	class ABag* Bag;

public:
	UPROPERTY(VisibleAnywhere)
	class AGun* CurrentGun = nullptr;

public:
	UFUNCTION()
	void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	AGun* GetMainGun();
	AGun* GetSubGun();

	void AttachMainGun();

	void AttachSubGun();

	FVector GetCameraLocation();
	FRotator GetCameraRotation();

	void TogglePerspective();

	void Aiming();
	UPROPERTY()
	bool IsAiming = false;

	UFUNCTION()
	void PlayFireMontage();
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FireMontage;

	bool LineTracingMouse(FHitResult& CameraHit);

	E_GroundPose GroundPose = E_GroundPose::EGP_Standing;

	const float baseMaxWeight = 20.f;
	UPROPERTY()
	float MaxWeight = 20.f;
	UPROPERTY()
	float CurrentWeight = 0.f;

	UPROPERTY()
	float CurrentVestDefence = 1.f;

	UPROPERTY()
	int32 currentAmmoCount[2];
	UPROPERTY()
	TArray<class AItem*> havingItems;

	//Inventory
	UPROPERTY()
	TSubclassOf<UUserWidget> InventoryWidgetClass;
	UPROPERTY()
	class UInventory* InventoryWidget;

	UPROPERTY()
	bool IsInventoryOpen = false;

	UFUNCTION()
	void RefreshInventory();
	

private:
	float currentHP;
	const float maxHP = 100.f;

public:
	float GetHP();

	float GetCurrentGunDamage();
	float GetCurrentVestDefence();

	void TakeDamege(bool isHeadShot, float damage, float reduceDamage);
	void HealHP(float AmountOfRecovery);

	void UseItem(AItem* item);
};
