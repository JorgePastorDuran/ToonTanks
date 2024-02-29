#include "HealthComponent.h"
#include "Tank.h"
#include "Tower.h"
#include "GameModeBaseLevel1ToonTanks.h"
#include "BasePawn.h"
#include "ToonTanksPlayerController.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
}


void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	Health -= Damage;

	if (Health <= 0)
	{
		Health = MaxHealth;

		AGameModeBase* GameModeBase = GetWorld()->GetAuthGameMode();
		AGameModeBaseLevel1ToonTanks* GameModeLevel1 = Cast<AGameModeBaseLevel1ToonTanks>(GameModeBase);
		ABasePawn* BasePawnActor = Cast<ABasePawn>(GetOwner());
		if (!BasePawnActor)
		{
			return;
		}
		ToonTanksPlayerControllerRef = Cast<AToonTanksPlayerController>(BasePawnActor->GetLocalViewingPlayerController());
		Tank = Cast<ATank>(GetOwner());
		Tower = Cast<ATower>(GetOwner());

		if (GameModeLevel1 && Tank)
		{
			ToonTanksPlayerControllerRef->SetPlayerEnabledState(false);
			Tank->HandleDestructionEffect();
			GameModeLevel1->reloadLevel();
			return;
		}
		else if (GameModeLevel1 && Tower)
		{
			GetOwner()->SetActorHiddenInGame(true);
			GetOwner()->SetActorEnableCollision(false);
			Tower->bEnableTower(false);
			Tower->HandleDestructionEffect();
			Tower->Destroy();
			if (GameModeLevel1->GetNumberOfTowers()<=0)
			{
				GameModeLevel1->EndGame(true);
				return;
			}
		}
		return;
	}
	return;
}

