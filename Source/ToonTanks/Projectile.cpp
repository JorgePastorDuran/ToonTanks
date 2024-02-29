// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshProjectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = StaticMeshProjectile;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->InitialSpeed = 3000.f;
	EmitterEffectProjectileTrail = CreateDefaultSubobject<USceneComponent>(TEXT("Emitter Effect"));
	EmitterEffectProjectileTrail->SetupAttachment(StaticMeshProjectile);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	StaticMeshProjectile->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	if (AudioExplote) 
	{ 
		UGameplayStatics::PlaySoundAtLocation(this, AudioProjectileStart, GetActorLocation()); 
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetOwner()) 
	{ 
		return; 
	}
	const FDamageEvent DamageEvent;
	AController* InstigatorControllerBasePawn = GetOwner()->GetInstigatorController();
	float damage = OtherActor->TakeDamage(Damage, DamageEvent, InstigatorControllerBasePawn, this);
	if (HitEmmiter && AudioExplote && CameraHit)
	{	
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEmmiter, GetTransform(), true);
		UGameplayStatics::PlaySoundAtLocation(this, AudioExplote, GetActorLocation());
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(CameraHit);
	}
	Destroy();
}
 
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UGameplayStatics::SpawnEmitterAttached(ProjectileTrailEmmiter, EmitterEffectProjectileTrail, name, FVector(0, 0, 0), FRotator(0,0,0));
}

