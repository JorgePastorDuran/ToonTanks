// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	class UStaticMeshComponent* StaticMeshProjectile;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* EmitterEffectProjectileTrail;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float Damage = 50.f;

	FName name = FName();

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* HitEmmiter;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* ProjectileTrailEmmiter;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* AudioExplote;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* AudioProjectileStart;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UCameraShakeBase> CameraHit;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
