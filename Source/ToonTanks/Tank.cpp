// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Tower.h"

ATank::ATank()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComp->SetupAttachment(BaseMesh);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComp->SetupAttachment(SpringArmComp);

	ParticleEmmiterPointMoveLeft = CreateDefaultSubobject<USceneComponent>(TEXT("Point Emitter Left Movement"));
	ParticleEmmiterPointMoveLeft->SetupAttachment(BaseMesh);

	ParticleEmmiterPointMoveRight = CreateDefaultSubobject<USceneComponent>(TEXT("Point Emitter Right Movement"));
	ParticleEmmiterPointMoveRight->SetupAttachment(BaseMesh);
}


// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Rotate);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	TankPlayerControllerRef = Cast<APlayerController>(this->GetLocalViewingPlayerController());
	TankPlayerControllerRef->SetShowMouseCursor(true);
}


void ATank::Move(float Value)
{
	if (Value!= 0)
	{		
		UGameplayStatics::SpawnEmitterAttached(MoveTankEmmiter, ParticleEmmiterPointMoveLeft);
		UGameplayStatics::SpawnEmitterAttached(MoveTankEmmiter, ParticleEmmiterPointMoveRight);

		if (Value>0)
		{
			AddActorLocalOffset(FVector::ForwardVector * speed * UGameplayStatics::GetWorldDeltaSeconds(this), true);
			return;
		}
		AddActorLocalOffset(FVector::BackwardVector * speed * UGameplayStatics::GetWorldDeltaSeconds(this), true);
	}

}

void ATank::Rotate(float Value)
{
	if (Value != 0)
	{
		if (Value > 0)
		{
			AddActorLocalRotation(Turn * Value* speed * GameplayStaticComp->GetWorldDeltaSeconds(this), true); //mirar en el .h
			return;
		}
		AddActorLocalRotation(Turn * Value * speed * UGameplayStatics::GetWorldDeltaSeconds(this), true);
	}
}


// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TankPlayerControllerRef)
	{
		return;
	}
	bool HitObject = TankPlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	if (HitObject)
	{
		if (HitResult.GetActor()->ActorHasTag("TankTag"))
		{
			return;
		}
		RotateTurretTo(HitResult.ImpactPoint);
	}
}

void ATank::DrawSphereCursor()
{
		UWorld* world = GetWorld();
		FVector Center;
		float Radius = 20.f;
		int32 Segments = 8;
		DrawDebugSphere(world, HitResult.ImpactPoint, Radius, Segments, FColor::Blue, false, -1);
}

void ATank::HandleDestructionEffect()
{
	Super::HandleDestructionEffect();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false); 
	if (CameraDeath)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(CameraDeath);
	}
	UE_LOG(LogTemp, Warning, TEXT("Effects Destruction Children"));
}

