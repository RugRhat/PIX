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
	// Manually bind buttons.
	UPROPERTY(meta = (BindWidget))
	class UButton* PlayButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* EnterNameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinServerButton;

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

	// Joins server running on player specified IP Address.
	UFUNCTION()
	void JoinServer();

	// Menu switching functions.
	UFUNCTION()
	void OpenPlayerName();

	UFUNCTION()
	void OpenMainMenu();

	// Takes player to Join Menu where they can enter host server's IP address.
	UFUNCTION()
	void OpenJoinMenu();

	// Assigns player specified player name.
	UFUNCTION()
	void SetPlayerName();

	UFUNCTION()
	void QuitGame();

protected: 
	virtual bool Initialize();
};
