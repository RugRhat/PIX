// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PIX_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

private:
	// Button pressed sound.
	UPROPERTY(EditAnywhere)
	class USoundBase* ButtonPressed;
	
	// Manually bind buttons.
	UPROPERTY(meta = (BindWidget))
	class UButton* PlayButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* EnterNameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SinglePlayerButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinServerButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectCharButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;				// Main quit button.

	UPROPERTY(meta = (BindWidget))
	class UButton* PNSQuitButton;			// Enter username quit button.

	UPROPERTY(meta = (BindWidget))
	class UButton* MMBackButton;			// Main Menu back button.

	UPROPERTY(meta = (BindWidget))
	class UButton* JMBackButton;			// Join Menu back button.

	// Manually bind widget switcher.
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	// Manually bind menu widgets.
	UPROPERTY(meta = (BindWidget))
	class UWidget* LaunchScreen;

	UPROPERTY(meta = (BindWidget))
	class UWidget* PlayerNameScreen;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	// Manually bind text box.
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* PlayerName;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IPAddress;
	
	UFUNCTION()
	void HostServer();

	// Joins server running on player specified IPAddress.
	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void QuitGame();	

	// Menu switching functions.
	UFUNCTION()
	void OpenPlayerName();

	UFUNCTION()
	void OpenMainMenu();

	// Takes player to Join Menu where they can enter host server's IP address.
	UFUNCTION()
	void OpenJoinMenu();

	// Returns to player name widget screen.
	UFUNCTION()
	void ReturnToPlayerName();

	// Returns to main menu widget screen.
	UFUNCTION()
	void ReturnToMain();

	// Assigns player specified player name.
	UFUNCTION()
	void SetPlayerName();

protected: 
	virtual bool Initialize();
};
