#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeBaseLevel1ToonTanks.generated.h"

UCLASS()
class TOONTANKS_API AGameModeBaseLevel1ToonTanks : public AGameModeBase
{
	GENERATED_BODY()

public:
	void reloadLevel();
	int GetNumberOfTowers();

	UFUNCTION(BlueprintImplementableEvent)
	void EndGame(bool DestroyAllEnemies);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

private:
	float StartDelay = 3.f;
	class AToonTanksPlayerController* ToonTanksPlayerControllerRef;
	FTimerHandle TimerHandleStartGame;
	FTimerHandle TimerHandleRestartGame;
	bool restart = true;
};
