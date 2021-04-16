// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"
#include "DefaultPlayerState.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "PIXGameInstance.h"


bool UMainMenu::Initialize() 
{
    bool Success = Super::Initialize();
    if(!Success) return false;

    if(!ensure(PlayButton != nullptr)) return false;
    PlayButton->OnClicked.AddDynamic(this, &UMainMenu::OpenPlayerName);

    if(!ensure(EnterNameButton != nullptr)) return false;
    EnterNameButton->OnClicked.AddDynamic(this, &UMainMenu::SetPlayerName);

    if(!ensure(SinglePlayerButton != nullptr)) return false;
    SinglePlayerButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

    if(!ensure(HostButton != nullptr)) return false;
    HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

    if(!ensure(JoinButton != nullptr)) return false;
    JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

    if(!ensure(JoinServerButton != nullptr)) return false;
    JoinServerButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

    if(!ensure(QuitButton != nullptr)) return false;
    QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

    if(!ensure(PNSQuitButton != nullptr)) return false;
    PNSQuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

    if(!ensure(MMBackButton != nullptr)) return false;
    MMBackButton->OnClicked.AddDynamic(this, &UMainMenu::ReturnToPlayerName);

    if(!ensure(JMBackButton != nullptr)) return false;
    JMBackButton->OnClicked.AddDynamic(this, &UMainMenu::ReturnToMain);

    return true;
}

// Assigns user specified player name.
void UMainMenu::SetPlayerName() 
{
    UPIXGameInstance* GameInstance = Cast<UPIXGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if(!ensure(GameInstance != nullptr)) return;
    if(!ensure(PlayerName != nullptr)) return;

    PlaySound(ButtonPressed);

    FString Name = PlayerName->GetText().ToString();
    GameInstance->SetPlayerName(Name);

    OpenMainMenu();
}

void UMainMenu::HostServer() 
{
    PlaySound(ButtonPressed);
    
    if(MenuInterface != nullptr)
    {

        MenuInterface->Host();
    }
}

// Joins server running on player specified IPAddress.
void UMainMenu::JoinServer() 
{
    PlaySound(ButtonPressed);

    if(MenuInterface != nullptr)
    {
        if(!ensure(IPAddress != nullptr)) return;

        FString Address = IPAddress->GetText().ToString();
        MenuInterface->Join(Address);
    }
}

void UMainMenu::OpenPlayerName() 
{
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(PlayerNameScreen != nullptr)) return;

    PlaySound(ButtonPressed);

    MenuSwitcher->SetActiveWidget(PlayerNameScreen);
}

void UMainMenu::OpenMainMenu() 
{
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(MainMenu != nullptr)) return;

    PlaySound(ButtonPressed);

    MenuSwitcher->SetActiveWidget(MainMenu);
}

// Takes player to Join Menu where they can enter host server's IP address.
void UMainMenu::OpenJoinMenu() 
{
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(JoinMenu != nullptr)) return;

    PlaySound(ButtonPressed);

    MenuSwitcher->SetActiveWidget(JoinMenu);
}

// Returns to player name widget screen.
void UMainMenu::ReturnToPlayerName() 
{
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(PlayerNameScreen != nullptr)) return;

    PlaySound(ButtonPressed);

    MenuSwitcher->SetActiveWidget(PlayerNameScreen);
}

// Takes player back to Main Menu.
void UMainMenu::ReturnToMain() 
{
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(MainMenu != nullptr)) return;

    PlaySound(ButtonPressed);

    MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitGame() 
{
    PlaySound(ButtonPressed);

    UWorld* World = GetWorld();
    if(!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if(!ensure(PlayerController != nullptr)) return;

    PlayerController->ConsoleCommand("quit");
}

