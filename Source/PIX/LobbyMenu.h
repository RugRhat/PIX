// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "LobbyMenu.generated.h"

// Called when game mode is selected.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameModeSelected);

UCLASS()
class PIX_API ULobbyMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	// Event called when game mode is selected that is assigned through blueprint.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay")
	FOnGameModeSelected GameModeSelected;

	// Button pressed sound.
	UPROPERTY(EditAnywhere)
	class USoundBase* ButtonPressed;

	// Manually bind buttons.
	UPROPERTY(meta = (BindWidget))
	class UButton* HordeButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* FFAButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* TDMButton;

	// UPROPERTY(meta = (BindWidget))
	// class UButton* TeamOneButton;

	// UPROPERTY(meta = (BindWidget))
	// class UButton* TeamTwoButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CustomizeCharButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* SelectGMButton;

	// UPROPERTY(meta = (BindWidget))
	// class UButton* ChangeGMButton;

	// UPROPERTY(meta = (BindWidget))
	// class UButton* TDMChangeGMButton;

	// UPROPERTY(meta = (BindWidget))
	// class UButton* ChangeTeamButton;

	// UPROPERTY(meta = (BindWidget))
	// class UButton* PlayButton;

	// UPROPERTY(meta = (BindWidget))
	// class UButton* TDMPlayButton;

	// Manually bind widget switcher.
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	// Manually bind menu widgets.
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* SelectGameMode;

	UPROPERTY(meta = (BindWidget))
	class UWidget* CustomizeCharacter;

	// UPROPERTY(meta = (BindWidget))
	// class UWidget* TDMChooseTeam;

	// UPROPERTY(meta = (BindWidget))
	// class UWidget* StartGame;

	// UPROPERTY(meta = (BindWidget))
	// class UWidget* TDMStartGame;

	// Tells menu interface to server travel to selected game mode map.
	UFUNCTION(BlueprintCallable, Category = "Game")
	void BeginGame();

	// Set Horde as the active game mode. Triggers next widget screen.
	UFUNCTION()
	void SetHordeGM();

	// Set Free For All as the active game mode. Triggers next widget screen.
	UFUNCTION()
	void SetFFAGM();

	// Set Team Death Match as the active game mode. Triggers next widget screen.
	UFUNCTION()
	void SetTDMGM();

	// UFUNCTION()
	// void SetTeamOne();

	// UFUNCTION()
	// void SetTeamTwo();

	UFUNCTION()
	void OpenCharCustomizer();

	UFUNCTION()
	void OpenGMSelect();
	
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ReturnToMenu();

	// // Returns to choose game mode widget screen.
	// UFUNCTION()
	// void ReturnToChooseGM();
	
	// // Returns to choose team widget screen.
	// UFUNCTION()
	// void ReturnToChooseTeam();

	// void SetTeam(int Team);

	// // Keeps track of how many players have joined each team.
	// int TeamOne = 0;
	// int TeamTwo = 0;

	virtual bool Initialize();
};
