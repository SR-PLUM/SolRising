// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Solaris.generated.h"

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
	void Pick();

private:
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* ViewCamera;

	UPROPERTY()
	TArray<class AItem*> OverlappedItem;

	UPROPERTY(VisibleAnywhere)
	class AGun* MainGun = nullptr;
	UPROPERTY(VisibleAnywhere)
	class AGun* SubGun = nullptr;

public:
	UFUNCTION()
	void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	AGun* GetMainGun();
};
