// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Gun.generated.h"

UENUM(BlueprintType)
enum class E_GunName : uint8
{
	EGN_M416 UMETA(DisplayName = "M416"),
	EGN_AK74U UMETA(DisplayName = "AK74U"),
	EGN_AK47 UMETA(DisplayName = "AK47"),
	EGN_Null UMETA(DisplayName = "Null State"),
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

private:
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* GunMesh;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USceneComponent* MuzzleLocation;
	UPROPERTY(EditAnywhere)
		class UStaticMesh* M416Mesh;
	UPROPERTY(EditAnywhere)
		class UStaticMesh* AK74UMesh;
	UPROPERTY(EditAnywhere)
		class UStaticMesh* AK47Mesh;

	UPROPERTY()
	UTexture2D* M416Img;
	UPROPERTY()
	UTexture2D* AK74UImg;
	UPROPERTY()
	UTexture2D* AK47Img;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AAmmoProjectile> AmmoProjectileActor;

private:					
	//M416
	float M416Damage = 10.0;
	float M416RPM;
	float M416VerticalRecoil;
	float M416HorizontalRecoil;

	UPROPERTY(EditAnywhere)
		USoundBase* M416FireSound;

	UPROPERTY(EditAnywhere)
		UAnimMontage* M416FireAnimation;

	//SCAR
	float AK74UDamage = 20.0;
	float AK74URPM;
	float AK74UVerticalRecoil;
	float AK74UHorizontalRecoil;

	UPROPERTY(EditAnywhere)
		USoundBase* AK74UFireSound;

	UPROPERTY(EditAnywhere)
		UAnimMontage* AK74UFireAnimation;
	
	//AK47
	float AK47Damage = 30.0;
	float AK47RPM;
	float AK47VerticalRecoil;
	float AK47HorizontalRecoil;

	UPROPERTY(EditAnywhere)
		USoundBase* AK47FireSound;

	UPROPERTY(EditAnywhere)
		UAnimMontage* AK47FireAnimation;

public:
	E_FireMethod currentFireMethod = E_FireMethod::EFM_Semi_Auto;
	E_GunName currentGunName = E_GunName::EGN_Null;

public:
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

	UPROPERTY()
	class ASolaris* OwningCharacter = nullptr;

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
	void SetOwningCharacter(ASolaris* owningCharacter);

public:
	int32 GetLoadedAmmo();
	float GetAmmoWeight();
	int32 GetAmmoType();
	FText GetGunName();
};
