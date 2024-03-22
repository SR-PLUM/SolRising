// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Gun.generated.h"

UENUM(BlueprintType)
enum class E_GunName : uint8
{
	EGN_M416 UMETA(DisplayName = "M416"),
	EGN_SCAR UMETA(DisplayName = "SCAR"),
	EGN_AK47 UMETA(DisplayName = "AK47"),
};

UENUM(BlueprintType)
enum E_FireMethod : uint8
{
	EFM_Semi_Auto UMETA(DisplayName = "Semi Auto"),
	EFM_Full_Auto UMETA(DisplayName = "Full Auto"),
};

UCLASS()
class SOLRISING_API AGun : public AItem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Picked() override;

private:
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* GunMesh;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USceneComponent* MuzzleLocation;

private:					
	//M416
	float M416Damage;
	float M416RPM;
	float M416VerticalRecoil;
	float M416HorizontalRecoil;

	UPROPERTY(EditAnywhere)
		USoundBase* M416FireSound;

	UPROPERTY(EditAnywhere)
		UAnimMontage* M416FireAnimation;

	//SCAR
	float SCARDamage;
	float SCARRPM;
	float SCARVerticalRecoil;
	float SCARHorizontalRecoil;

	UPROPERTY(EditAnywhere)
		USoundBase* SCARFireSound;

	UPROPERTY(EditAnywhere)
		UAnimMontage* SCARFireAnimation;
	
	//AK47
	float AK47Damage;
	float AK47RPM;
	float AK47VerticalRecoil;
	float AK47HorizontalRecoil;

	UPROPERTY(EditAnywhere)
		USoundBase* AK47FireSound;

	UPROPERTY(EditAnywhere)
		UAnimMontage* AK47FireAnimation;

public:
	E_FireMethod currentFireMethod = E_FireMethod::EFM_Semi_Auto;

protected:
	float damage;
	float RPM;				//Rounds per minute
	float verticalRecoil;
	float horizontalRecoil;

	UPROPERTY(EditAnywhere)
		USoundBase* FireSound;

	UPROPERTY(EditAnywhere)
		UAnimMontage* FireAnimation;

private:
	bool isReloading = false;
	bool isFiring = false;
	bool canFire = true;
	
	int32 loadedAmmo;		//Remain ammo in current magazine
	int32 remainAmmo;		//Total ammo except already loaded
	int32 magazine;			//Max ammo in one magaznie

	float TraceDistance = 10000.f;

	FVector GunOffset = FVector(100.0f, 0.0f, 10.0f);

private:
	FTimerHandle reloadTimer;
	FTimerHandle fireTimer;

public:
	void SelectiveFire();
	void OnFire();
	void Fire();
	void FireCooldown();
	void StopFire();
	void Reroad();
	void ReloadDelay();
	void Aiming();
	void AttachMeshToSocket(USceneComponent* InParent, const FName& SocketName);
};
