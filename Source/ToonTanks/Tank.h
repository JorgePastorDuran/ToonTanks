// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"


/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()
	
public:

	ATank();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleDestructionEffect();


protected:
	virtual void BeginPlay() override;

	APlayerController* TankPlayerControllerRef;

private:

	UPROPERTY(VisibleAnywhere,  Category = "Components")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere,  Category = "Movement")
	USceneComponent* ParticleEmmiterPointMoveLeft;

	UPROPERTY(VisibleAnywhere,  Category = "Movement")
	USceneComponent* ParticleEmmiterPointMoveRight;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float speed = 400.f;

	void Move(float Value);
	void Rotate(float Value);
	void DrawSphereCursor();

	class UGameplayStatics* GameplayStaticComp;

	UPROPERTY(EditAnywhere, Category = "Movement")
	FRotator Turn = FRotator(0, 0.5, 0);

	FHitResult HitResult;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UCameraShakeBase> CameraDeath;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* MoveTankEmmiter;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
