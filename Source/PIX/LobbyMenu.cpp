// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "HealthComponent.h"
#include "PIXGameInstance.h"


bool ULobbyMenu::Initialize() 
{
    bool Success = Super::Initialize();
    if(!Success) return false;

    if(!ensure(HordeButton != nullptr)) return false;
    HordeButton->OnClicked.AddDynamic(this, &ULobbyMenu::SetHordeGM);

    if(!ensure(FFAButton != nullptr)) return false;
    FFAButton->OnClicked.AddDynamic(this, &ULobbyMenu::SetFFAGM);

    if(!ensure(TDMButton != nullptr)) return false;
    TDMButton->OnClicked.AddDynamic(this, &ULobbyMenu::SetTDMGM);

    // if(!ensure(TeamOneButton != nullptr)) return false;
    // TeamOneButton->OnClicked.AddDynamic(this, &ULobbyMenu::SetTeamOne);

    // if(!ensure(TeamTwoButton != nullptr)) return false;
    // TeamTwoButton->OnClicked.AddDynamic(this, &ULobbyMenu::SetTeamTwo);

    if(!ensure(CustomizeCharButton != nullptr)) return false;
    CustomizeCharButton->OnClicked.AddDynamic(this, &ULobbyMenu::OpenCharCustomizer);

    if(!ensure(SelectGMButton != nullptr)) return false;
    SelectGMButton->OnClicked.AddDynamic(this, &ULobbyMenu::OpenGMSelect);

    // if(!ensure(TDMChangeGMButton != nullptr)) return false;
    // TDMChangeGMButton->OnClicked.AddDynamic(this, &ULobbyMenu::ReturnToChooseGM);

    // if(!ensure(ChangeTeamButton != nullptr)) return false;
    // ChangeTeamButton->OnClicked.AddDynamic(this, &ULobbyMenu::ReturnToChooseTeam);

    // if(!ensure(PlayButton != nullptr)) return false;
    // PlayButton->OnClicked.AddDynamic(this, &ULobbyMenu::BeginGame);

    // if(!ensure(TDMPlayButton != nullptr)) return false;
    // TDMPlayButton->OnClicked.AddDynamic(this, &ULobbyMenu::BeginGame);

    return true;
}

// Tells menu interface to server travel to selected game mode map.
void ULobbyMenu::BeginGame() 
{
    if(MenuInterface != nullptr)
    {
        MenuInterface->LoadGame();
    }
}

// Set Horde as the active game mode. Triggers next widget screen.
void ULobbyMenu::SetHordeGM() 
{
    if(MenuInterface != nullptr)
    {
        MenuInterface->SetGameMode("/Game/Maps/GameModes/Horde?game=/Game/Blueprints/GameModes/BP_HordeGameMode.BP_HordeGameMode_C?listen");
        MenuInterface->SetGameModeID("Horde");
    }

    GameModeSelected.Broadcast();

    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(MainMenu != nullptr)) return;

    MenuSwitcher->SetActiveWidget(MainMenu);
}

// Set Free For All as the active game mode. Triggers next widget screen.
void ULobbyMenu::SetFFAGM() 
{
    if(MenuInterface != nullptr)
    {
        MenuInterface->SetGameMode("/Game/Maps/GameModes/FreeForAll?game=/Game/Blueprints/GameModes/BP_FFAGameMode.BP_FFAGameMode_C?listen");
        MenuInterface->SetGameModeID("FFA");
    }

    GameModeSelected.Broadcast();

    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(MainMenu != nullptr)) return;

    MenuSwitcher->SetActiveWidget(MainMenu);
}

// Set Team Death Match as the active game mode. Triggers next widget screen.
void ULobbyMenu::SetTDMGM() 
{
    if(MenuInterface != nullptr)
    {
        MenuInterface->SetGameMode("/Game/Maps/GameModes/TeamDeathMatch?game=/Game/Blueprints/GameModes/BP_TDMGameMode.BP_TDMGameMode_C?listen");
        MenuInterface->SetGameModeID("TDM");
    }

    GameModeSelected.Broadcast();
    
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(MainMenu != nullptr)) return;

    MenuSwitcher->SetActiveWidget(MainMenu);
}

void ULobbyMenu::OpenCharCustomizer() 
{
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(CustomizeCharacter != nullptr)) return;

    PlaySound(ButtonPressed);

    MenuSwitcher->SetActiveWidget(CustomizeCharacter);
}

void ULobbyMenu::OpenGMSelect() 
{
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(SelectGameMode != nullptr)) return;

    PlaySound(ButtonPressed);
    
    MenuSwitcher->SetActiveWidget(SelectGameMode);
}

void ULobbyMenu::ReturnToMenu() 
{
    if(!ensure(MenuSwitcher != nullptr)) return;
    if(!ensure(MainMenu != nullptr)) return;

    MenuSwitcher->SetActiveWidget(MainMenu);
}

// void ULobbyMenu::SetTeam(int Team) 
// {
//     APlayerController* PlayerController = Cast<APlayerController>(GetOwningPlayer());

//     if(ensure(PlayerController))
//     {
//         if(PlayerController->GetPawn())
//         {
//             UPIXGameInstance* GameInstance = Cast<UPIXGameInstance>(PlayerController->GetPawn()->GetGameInstance());
//             GameInstance->SetPlayerTeam(Team);
//         }
//     }

//     if(!ensure(MenuSwitcher != nullptr)) return;
//     if(!ensure(TDMStartGame != nullptr)) return;

//     MenuSwitcher->SetActiveWidget(TDMStartGame);
// }

// void ULobbyMenu::SetTeamOne() 
// {
//     /// TODO: Set max team no. to be half of active players.
//     if(TeamOne != 4)
//     {
//         SetTeam(1);
//         TeamOne ++;
//     }
//     else
//     {
//         /// TODO: Display team full error message.
//     }
// }

// void ULobbyMenu::SetTeamTwo() 
// {
//     /// TODO: Set max team no. to be half of active players.
//     if(TeamTwo != 4)    
//     {
//         SetTeam(2);
//         TeamTwo ++;
//     }
//     else
//     {
//         /// TODO: Display team full error message.
//     }
// }

// // Returns to choose game mode widget screen.
// void ULobbyMenu::ReturnToChooseGM() 
// {
//     if(!ensure(MenuSwitcher != nullptr)) return;
//     if(!ensure(ChooseGameMode != nullptr)) return;

//     MenuSwitcher->SetActiveWidget(ChooseGameMode);
// }

// // Returns to choose team widget screen.
// void ULobbyMenu::ReturnToChooseTeam() 
// {
//     if(!ensure(MenuSwitcher != nullptr)) return;
//     if(!ensure(TDMChooseTeam != nullptr)) return;

//     MenuSwitcher->SetActiveWidget(TDMChooseTeam);
// }
