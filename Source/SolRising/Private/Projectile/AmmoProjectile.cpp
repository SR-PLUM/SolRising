// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/AmmoProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AAmmoProjectile::AAmmoProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AmmoProjectileSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AmmoProjectile Sphere"));
	RootComponent = AmmoProjectileSphere;

	AmmoProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoProjectile Mesh"));
	AmmoProjectileMesh->SetupAttachment(GetRootComponent());

	AmmoProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("AmmoProjectile Component"));
	AmmoProjectileComponent->UpdatedComponent = AmmoProjectileMesh;
	AmmoProjectileComponent->ProjectileGravityScale = 0;
	AmmoProjectileComponent->InitialSpeed =1000;
	AmmoProjectileComponent->MaxSpeed = 1000;
	AmmoProjectileComponent->bRotationFollowsVelocity = true;
	AmmoProjectileComponent->bShouldBounce = false;

	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AAmmoProjectile::BeginPlay()
{
	Super::BeginPlay();

	AmmoProjectileSphere->OnComponentHit.AddDynamic(this, &AAmmoProjectile::OnHit);
}

// Called every frame
void AAmmoProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	Destroy();
}

