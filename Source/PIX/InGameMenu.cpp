// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"


bool UInGameMenu::Initialize() 
{
    bool Success = Super::Initialize();
    if(!Success) return false;

    if(!ensure(QuitButton != nullptr)) return false;
    QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitGame);

    if(!ensure(BackToLobbyButton != nullptr)) return false;
    BackToLobbyButton->OnClicked.AddDynamic(this, &UInGameMenu::BackToLobby);

    if(!ensure(BackButton != nullptr)) return false;
    BackButton->OnClicked.AddDynamic(this, &UMenuWidget::RemoveMenu);

    return true;
}

// Takes server back to lobby.
void UInGameMenu::BackToLobby() 
{
    if(MenuInterface != nullptr)
    {
        MenuInterface->BackToLobby();
    }

    RemoveMenu();
}

void UInGameMenu::QuitGame() 
{
    UWorld* World = GetWorld();
    if(!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if(!ensure(PlayerController != nullptr)) return;

    PlayerController->ConsoleCommand("quit");

    RemoveMenu();
}

