// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"


void UMenuWidget::SetMenuInterface(IMenuInterface* MenuInt) 
{
    this->MenuInterface = MenuInt;
}

// Adds widget to screen and allows for mouse control.
void UMenuWidget::Setup() 
{
    this->AddToViewport();

    UWorld* World = GetWorld();
    if(!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if(!ensure(PlayerController != nullptr)) return;

    // Allows player to click.
    FInputModeUIOnly InputModeData;
    InputModeData.SetWidgetToFocus(this->TakeWidget());

    // Allows cursor to move around screen.
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);        

    PlayerController->SetInputMode(InputModeData);

    // Makes mouse visible.
    PlayerController->bShowMouseCursor = true;
}

// Removes widget & mouse from viewport.
void UMenuWidget::RemoveMenu() 
{
    this->RemoveFromViewport();

    UWorld* World = GetWorld();
    if(!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if(!ensure(PlayerController != nullptr)) return;

    FInputModeGameOnly InputMode;
    PlayerController->SetInputMode(InputMode);

    PlayerController->bShowMouseCursor = false;
}
