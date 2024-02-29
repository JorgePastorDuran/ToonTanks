// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBaseLevel1ToonTanks.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksPlayerController.h"
#include "Engine/TimerHandle.h"
#include "Tower.h"

void AGameModeBaseLevel1ToonTanks::reloadLevel()
{
	if (restart)
	{
		restart = false;
		EndGame(false);
		GetWorldTimerManager().SetTimer(TimerHandleRestartGame, this, &AGameModeBaseLevel1ToonTanks::reloadLevel, 3.f, false, 3.f);
		return;
	}
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AGameModeBaseLevel1ToonTanks::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("The number of towers is: %d"), GetNumberOfTowers());

	StartGame();
	ToonTanksPlayerControllerRef = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!ToonTanksPlayerControllerRef) 
	{ 
		return; 
	}
	ToonTanksPlayerControllerRef->SetPlayerEnabledState(false);
	FTimerDelegate InputDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerControllerRef, &AToonTanksPlayerController::SetPlayerEnabledState,true);
	GetWorldTimerManager().SetTimer(TimerHandleStartGame, InputDelegate, 3.f, false, StartDelay);
}

int AGameModeBaseLevel1ToonTanks::GetNumberOfTowers()
{
	ATower* tower;
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, tower->StaticClass(), OutActors);
	return OutActors.Num();
}