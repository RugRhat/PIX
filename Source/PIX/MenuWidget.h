// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PIX_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* MenuInt);

	// Adds widget to screen and allows for mouse control.
	void Setup();

	// Removes widget & mouse from viewport.
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void RemoveMenu();

protected:
	IMenuInterface* MenuInterface;	
};
