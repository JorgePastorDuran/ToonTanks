// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h"
#include "HealthComponent.h"	
#include "Tower.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

}

void ABasePawn::HandleDestructionEffect()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEmmiter, GetTransform(), true);
	UGameplayStatics::PlaySoundAtLocation(this, AudioDeath, GetActorLocation());
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePawn::RotateTurretTo(FVector targetPosition)
{
	double distance = FVector::Distance(targetPosition, this->GetActorLocation());
	double Ydisntance = targetPosition.Y - this->GetTargetLocation().Y;
	double Xdisntance = targetPosition.X - this->GetTargetLocation().X;
	float AngleRadians = FMath::Atan2(Ydisntance, Xdisntance);
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
	UWorld* world = GetWorld();
	float InterpSpeed = 20.0f;
	Tower = Cast<ATower>(this);

	if (Tower)
	{
		double Zdistance = targetPosition.Z - this->GetTargetLocation().Z;
		float AngleRadians2 = FMath::Atan2(distance,Zdistance );
		float AngleDegrees2 = FMath::RadiansToDegrees(AngleRadians2);
		FRotator RotationTurretMesh = FRotator(0, AngleDegrees, AngleDegrees2-90);
		TurretMesh->SetWorldRotation(FMath::RInterpTo(TurretMesh->GetComponentRotation(), RotationTurretMesh, world->DeltaTimeSeconds, InterpSpeed));
	}
	else
	{
		FRotator RotationTurretMesh = FRotator(0, AngleDegrees, 0);
		TurretMesh->SetWorldRotation(FMath::RInterpTo(TurretMesh->GetComponentRotation(), RotationTurretMesh, world->DeltaTimeSeconds, InterpSpeed));
	}
	return;
}

void ABasePawn::Fire()
{
	UWorld* world = GetWorld();
	FCollisionQueryParams CollisionParams;
	ECollisionChannel CollisionChannel = ECC_Visibility;
	AProjectile* Projectile = world->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	Projectile->SetOwner(this);
}

