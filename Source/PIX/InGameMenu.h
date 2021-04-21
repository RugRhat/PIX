// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PIX_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	// Manually bind buttons.
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* BackToLobbyButton;		// Only host can access.

	UFUNCTION()
	void BackToLobby();

	UFUNCTION()
	void QuitGame();

	virtual bool Initialize();
};
