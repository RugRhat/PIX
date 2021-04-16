// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "PIXGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIX_API UPIXGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPIXGameInstance(const FObjectInitializer &ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	// UFUNCTION(BlueprintCallable)
	// void LoadWinScreen();

	UFUNCTION(Exec)
	void Single() override;

	UFUNCTION(Exec)
	void Host() override;

	UFUNCTION(Exec)
	void Join(const FString &Address) override;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	virtual void SetGameMode(const FString &GameMode) override;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	virtual void LoadGame() override;

	virtual void LoadMainMenu() override;

	virtual void Replay() override;	

	// UFUNCTION(BlueprintCallable, Category = "InGameMessages")
	// virtual void LoadDeathScreen() override;

	// UFUNCTION(BlueprintCallable, Category = "InGameMessages")
	// virtual void LoadRespawnScreen() override;

	// UFUNCTION(BlueprintCallable, Category = "InGameMessages")
	// virtual void LoadSpectateScreen() override;

protected:
	FString SelectedGame = "";

	// Game Menus.
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> WinScreenMenuClass;

	// In Game Messages.
	// TSubclassOf<class UUserWidget> DeathScreenClass;
	// TSubclassOf<class UUserWidget> RespawnScreenClass;
	// TSubclassOf<class UUserWidget> SpectateScreenClass;

	class UMainMenu* Menu;
};
