// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/AmmoProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Character/Solaris.h"

// Sets default values
AAmmoProjectile::AAmmoProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AmmoProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoProjectile Mesh"));
	RootComponent = AmmoProjectileMesh;

	AmmoProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("AmmoProjectile Component"));
	AmmoProjectileComponent->UpdatedComponent = AmmoProjectileMesh;
	AmmoProjectileComponent->ProjectileGravityScale = 0;
	AmmoProjectileComponent->InitialSpeed =10000;
	AmmoProjectileComponent->MaxSpeed = 0;
	AmmoProjectileComponent->bRotationFollowsVelocity = true;
	AmmoProjectileComponent->bShouldBounce = false;

	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AAmmoProjectile::BeginPlay()
{
	Super::BeginPlay();

	AmmoProjectileMesh->OnComponentHit.AddDynamic(this, &AAmmoProjectile::OnHit);
}

// Called every frame
void AAmmoProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == OwningCharacter)
		return;

	ASolaris* enemy = Cast<ASolaris>(OtherActor);	//AI로 변경
	if (enemy)
	{
		ASolaris* Solaris = Cast<ASolaris>(OwningCharacter);

		enemy->SetHP(enemy->GetHP() - Solaris->GetCurrentGunDamage());
		UE_LOG(LogTemp, Warning, TEXT("%f"), enemy->GetHP());
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	Destroy();
}

