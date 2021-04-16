// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"


bool UMainMenu::Initialize() 
{
    bool Success = Super::Initialize();
    if(!Success) return false;

    if(!ensure(PlayButton != nullptr)) return false;
    PlayButton->OnClicked.AddDynamic(this, &UMainMenu::OpenPlayerName);

    if(!ensure(EnterNameButton != nullptr)) return false;
    EnterNameButton->OnClicked.AddDynamic(this, &UMainMenu::SetPlayerName);

    if(!ensure(SelectCharButton != nullptr)) return false;
    SelectCharButton->OnClicked.AddDynamic(this, &UMainMenu::SetPlayerChar);

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
    MMBackButton->OnClicked.AddDynamic(this, &UMainMenu::ReturnToChooseChar);

    if(!ensure(CCSBackButton != nullptr)) return false;
    CCSBackButton->OnClicked.AddDynamic(this, &UMainMenu::ReturnToPlayerName);

    if(!ensure(JMBackButton != nullptr)) return false;
    JMBackButton->OnClicked.AddDynamic(this, &UMainMenu::ReturnToMain);

    return true;
}

void UMainMenu::SetPlayerName() 
{
    // if(MenuInterface != nullptr)
    // {
    //     if(!ensure(PlayerName != nullptr)) return;

    //     FString Name = PlayerName->GetText().ToString();
    //     MenuInterface->SetPlayerName(Name);
    // }

    OpenChooseCharScreen();
}

void UMainMenu::SetPlayerChar() 
{
    // if(MenuInterface != nullptr)
    // {
    //     if(!ensure(IPAddress != nullptr)) return;

    //     FString Address = IPAddress->GetText().ToString();
    //     MenuInterface->SetPlayerChar(Address);
    // }

    OpenMainMenu();
}

void UMainMenu::HostServer() 
{
    if(MenuInterface != nullptr)
    {
        MenuInterface->Host();
    }
}

void UMainMenu::JoinServer() 
{
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

    MenuSwitcher->SetActiveWidget(PlayerNameScreen);
}

void UMainMenu::OpenChooseCharScreen() 
{
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(ChooseCharScreen != nullptr)) return;

    MenuSwitcher->SetActiveWidget(ChooseCharScreen);
}

void UMainMenu::OpenMainMenu() 
{
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(MainMenu != nullptr)) return;

    MenuSwitcher->SetActiveWidget(MainMenu);
}

// Takes player to Join Menu where they can enter host server's IP address.
void UMainMenu::OpenJoinMenu() 
{
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(JoinMenu != nullptr)) return;

    MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::ReturnToPlayerName() 
{
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(PlayerNameScreen != nullptr)) return;

    MenuSwitcher->SetActiveWidget(PlayerNameScreen);
}

void UMainMenu::ReturnToChooseChar() 
{
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(ChooseCharScreen != nullptr)) return;

    MenuSwitcher->SetActiveWidget(ChooseCharScreen);
}

// Takes player back to Main Menu.
void UMainMenu::ReturnToMain() 
{
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(MainMenu != nullptr)) return;

    MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitGame() 
{
    UWorld* World = GetWorld();
    if(!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if(!ensure(PlayerController != nullptr)) return;

    PlayerController->ConsoleCommand("quit");
}

