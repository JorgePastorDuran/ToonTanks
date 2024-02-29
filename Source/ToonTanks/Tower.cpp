#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TimerHandle.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();
	world = GetWorld();
	OriginalRotation = TurretMesh->GetComponentRotation();
	MaxDistanceRangeShoot;
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0)); 
	bStartShoot = true;
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckAndHandleShooting();
	return;
}

void ATower::bEnableTower(bool enable)
{
	this->SetActorTickEnabled(enable);
	GetWorldTimerManager().ClearTimer(FireRateTimerHandle);
}

void ATower::CheckAndHandleShooting()
{
	if (Tank)
	{
		if (InFireRangeAndTargetValid())
		{
			RotateTurretTo(Tank->GetActorLocation());
			if (bStartShoot)
			{
				GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::Fire, 3.f, true, 3.f);
				bStartShoot = false;
			}
		}
		else {
			TurretMesh->SetWorldRotation(FMath::RInterpTo(TurretMesh->GetComponentRotation(), OriginalRotation, world->DeltaTimeSeconds, 2));
			GetWorldTimerManager().ClearTimer(FireRateTimerHandle);
			bStartShoot = true;
		}
	}
}

bool ATower::InFireRangeAndTargetValid()
{
		double DistanceTowerToPlayer = FVector::Distance(this->GetActorLocation(), Tank->GetActorLocation());

		if (MaxDistanceRangeShoot > DistanceTowerToPlayer)
		{
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);
			ECollisionChannel CollisionChannel = ECC_Visibility;
			FHitResult HitResult;
			world->LineTraceSingleByChannel(HitResult, this->GetActorLocation(), Tank->GetActorLocation(), CollisionChannel, CollisionParams);
			if (IsTargetObjectValid(HitResult))
			{
				return true;
			}
		}
	return false;
}

bool ATower::IsTargetObjectValid(FHitResult HitResult)
{
	if (HitResult.GetActor() && HitResult.GetActor()->ActorHasTag("TankTag")/*Cast<ATank>(HitResult.GetActor())*/)
	{
		return true;
	}
	return false;
}