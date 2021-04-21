// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "EndGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PIX_API UEndGameMenu : public UMenuWidget
{
	GENERATED_BODY()

public: 
	UPROPERTY(BlueprintReadOnly, Category = "Menu")
	bool IsWinner;
	
protected:
	// Manually bind buttons.
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* BackToLobbyButton;		// Only host can access.

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* ReplayButton;			// Only host can access.

	UFUNCTION()
	void BackToLobby();

	UFUNCTION()
	void Replay();

	UFUNCTION()
	void QuitGame();

	virtual bool Initialize();	
};
