// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	ABasePawn();

	void HandleDestructionEffect();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,  Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere,  Category = "Components")
	UStaticMeshComponent* TurretMesh;

	void RotateTurretTo(FVector);
	void Fire();

private:

	UPROPERTY(VisibleAnywhere,  Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent; 

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* DeathEmmiter;

	UPROPERTY(VisibleAnywhere,  Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* AudioDeath;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass; 

	UPROPERTY(VisibleAnywhere)
	class UHealthComponent* HealthComponent;

	class ATower* Tower;

};
