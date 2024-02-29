// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()

public:
	ATower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//Variables
	UWorld* world;
	FRotator OriginalRotation;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double MaxDistanceRangeShoot = 1500.f;

	class ATank* Tank;
	FTimerHandle FireRateTimerHandle;
	bool bStartShoot;
	

	void CheckAndHandleShooting();
	bool InFireRangeAndTargetValid();
	bool IsTargetObjectValid(FHitResult);



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void bEnableTower(bool enable);
};
