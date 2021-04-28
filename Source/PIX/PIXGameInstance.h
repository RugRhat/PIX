// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "PIXGameInstance.generated.h"

class AWeapon; 

UCLASS()
class PIX_API UPIXGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	// Sets default values for properties with initialization.
	UPIXGameInstance(const FObjectInitializer &ObjectInitializer);

	virtual void Init();

	// Owning player's team. Declared here to survive level change.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team")
	uint8 PlayerTeam;

	// Owning player's unique specified name. Declared here to survive level change.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	FString PlayerName;

	// // Owning player's chosen character. Declared here to survive level change.
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	// class USkeletalMesh* PlayerChar;		

	// // Owning player's chosen weapon. Declared here to survive level change.
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	// TSubclassOf<AWeapon> PlayerWeapon;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	// bool bCharChosen;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	// bool bWeaponChosen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	TArray<USkeletalMesh*> Characters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	TArray<TSubclassOf<AWeapon>> Weapons;

	// Loads main menu widget.
	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	// Loads lobby menu widget.
	UFUNCTION(BlueprintCallable)
	void LoadLobbyMenu();

	// Loads in-game menu widget.
	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	// Loads end game menu widget.
	UFUNCTION(BlueprintCallable)
	void LoadEndGameMenu(bool IsWinner);

	// // Sets owning player's chosen character.
	// UFUNCTION(BlueprintCallable, Category = "Player")
	// void SetPlayerCharacter(USkeletalMesh* ChosenCharacterClass);

	// // Sets owning player's chosen weapon.
	// UFUNCTION(BlueprintCallable, Category = "Player")
	// void SetPlayerWeapon(TSubclassOf<AWeapon> ChosenWeapon);
	
	// Return's active game mode ID.
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	FString GetGameModeID();

	// Takes host player to lobby.
	UFUNCTION(Exec)
	void Host() override;

	// Takes player to lobby on host's server using given IPAddress.
	UFUNCTION(Exec)
	void Join(const FString &Address) override;

	/// TODO: UFUNCTION?
	// Set's gamemode of future game to game mode chosen by host.
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	virtual void SetGameMode(const FString &GameMode) override;

	// Load's map and game mode which correspond's to the chosen game mode.
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	virtual void LoadGame() override;

	// Set's game mode ID to be used by health component to handle death events.
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	virtual void SetGameModeID(const FString &GameModeID) override;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	virtual void BackToLobby() override;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void RemoveLobbyMenu();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void RemoveInGameMenu();
	
	// Loads main menu map.
	virtual void LoadMainMenu() override;

	// "Replays" game by having the server re-travel to game mode map.
	virtual void Replay() override;	

	// Sets owning player's team.
	void SetPlayerTeam(int Team);

	// Sets owning player's unique specified name.
	void SetPlayerName(FString Name);

protected:
	FString SelectedGameMode = "";

	FString GMID = "";

	// Game Menus.
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> LobbyMenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;
	TSubclassOf<class UUserWidget> EndGameMenuClass;

	UPROPERTY(BlueprintReadOnly, category = "Menus")
	class UEndGameMenu* EndGameMenu;

	class UMainMenu* Menu;
	class ULobbyMenu* LobbyMenu;
	class UInGameMenu* InGameMenu;
};
