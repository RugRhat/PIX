// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameMenu.h"
#include "Components/Button.h"


bool UEndGameMenu::Initialize() 
{
    bool Success = Super::Initialize();
    if(!Success) return false;

    if(!ensure(QuitButton != nullptr)) return false;
    QuitButton->OnClicked.AddDynamic(this, &UEndGameMenu::QuitGame);

    if(!ensure(BackToLobbyButton != nullptr)) return false;
    BackToLobbyButton->OnClicked.AddDynamic(this, &UEndGameMenu::BackToLobby);

    if(!ensure(ReplayButton != nullptr)) return false;
    ReplayButton->OnClicked.AddDynamic(this, &UEndGameMenu::Replay);

    return true;
}

void UEndGameMenu::BackToLobby() 
{
    if(MenuInterface != nullptr)
    {
        MenuInterface->BackToLobby();
    }

    RemoveMenu();
}

void UEndGameMenu::Replay() 
{
    if(MenuInterface != nullptr)
    {
        MenuInterface->Replay();
    }

    RemoveMenu();
}

void UEndGameMenu::QuitGame() 
{
    UWorld* World = GetWorld();
    if(!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if(!ensure(PlayerController != nullptr)) return;

    PlayerController->ConsoleCommand("quit");

    RemoveMenu();
}

