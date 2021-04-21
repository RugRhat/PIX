// Fill out your copyright notice in the Description page of Project Settings.


#include "PIXGameInstance.h"
#include "Engine/Engine.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/World.h"
#include "EndGameMenu.h"
#include "InGameMenu.h"
#include "LobbyMenu.h"
#include "MainMenu.h"
#include "MenuWidget.h"
#include "PlayerCharacter.h"
#include "UObject/Class.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for properties with initialization.
UPIXGameInstance::UPIXGameInstance(const FObjectInitializer &ObjectInitializer) 
{
    // Allows for more control of Menu UI from source code rather than blueprints.
    ConstructorHelpers::FClassFinder<UUserWidget>MenuBPClass(TEXT("/Game/UI/Menus/WBP_MainMenu"));
    if(!ensure(MenuBPClass.Class != nullptr)) return;

    MenuClass = MenuBPClass.Class;

    // Allows for more control of Menu UI from source code rather than blueprints.
    ConstructorHelpers::FClassFinder<UUserWidget>LobbyMenuBPClass(TEXT("/Game/UI/Menus/WBP_LobbyMenu"));
    if(!ensure(LobbyMenuBPClass.Class != nullptr)) return;

    LobbyMenuClass = LobbyMenuBPClass.Class;

    // Allows for more control of Menu UI from source code rather than blueprints.
    ConstructorHelpers::FClassFinder<UUserWidget>InGameMenuBPClass(TEXT("/Game/UI/Menus/WBP_InGameMenu"));
    if(!ensure(InGameMenuBPClass.Class != nullptr)) return;

    InGameMenuClass = InGameMenuBPClass.Class;

    // Allows for more control of Menu UI from source code rather than blueprints.
    ConstructorHelpers::FClassFinder<UUserWidget>EndGameMenuBPClass(TEXT("/Game/UI/Menus/WBP_EndGameMenu"));
    if(!ensure(EndGameMenuBPClass.Class != nullptr)) return;

    EndGameMenuClass = EndGameMenuBPClass.Class;

    PlayerTeam = 1;

    bCharChosen = false;
}

// Sets up custom game instance properties.
void UPIXGameInstance::Init() 
{
    UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *MenuClass->GetName());
    UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *LobbyMenuClass->GetName());
}

// Loads menu.
void UPIXGameInstance::LoadMenu() 
{
    if(!ensure(MenuClass != nullptr)) return;

    Menu = CreateWidget<UMainMenu>(this, MenuClass);

    Menu->Setup();

    Menu->SetMenuInterface(this);
}

// Loads lobby menu widget.
void UPIXGameInstance::LoadLobbyMenu() 
{
    if(!ensure(LobbyMenuClass != nullptr)) return;

    LobbyMenu = CreateWidget<ULobbyMenu>(this, LobbyMenuClass);

    LobbyMenu->Setup();

    LobbyMenu->SetMenuInterface(this);
}

// Loads in-game menu widget.
void UPIXGameInstance::LoadInGameMenu() 
{
    if(!ensure(InGameMenuClass != nullptr)) return;

    InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);

    InGameMenu->Setup();

    InGameMenu->SetMenuInterface(this);
}

// Loads end game menu widget.
void UPIXGameInstance::LoadEndGameMenu(bool IsWinner) 
{
    if(!ensure(EndGameMenuClass != nullptr)) return;

    EndGameMenu = CreateWidget<UEndGameMenu>(this, EndGameMenuClass);

    EndGameMenu->IsWinner = IsWinner;

    EndGameMenu->Setup();

    EndGameMenu->SetMenuInterface(this);
}

// Sets owning player's chosen character.
void UPIXGameInstance::SetPlayerCharacter(USkeletalMesh* ChosenCharacter)
{
    bCharChosen = true;
    PlayerChar = ChosenCharacter;

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Player Character chosen"));
}

void UPIXGameInstance::SetPlayerWeapon(TSubclassOf<AWeapon>ChosenWeapon) 
{
    bWeaponChosen = true;
    PlayerWeapon = ChosenWeapon;

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Player weapon chosen"));
}

// Takes host player to lobby.
void UPIXGameInstance::Host() 
{
    if(Menu != nullptr)
    {
        Menu->RemoveMenu();
    }

    UEngine* Engine = GetEngine();
    if(!ensure(Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

    UWorld* World = GetWorld();
    if(!ensure(World != nullptr)) return;

    World->ServerTravel("/Game/Maps/Lobby?game=/Game/Blueprints/GameModes/BP_LobbyGameMode.BP_LobbyGameMode_C?listen");      // Switches to Lobby map.
}

// Takes player to lobby on host's server using given IPAddress.
void UPIXGameInstance::Join(const FString &Address) 
{
    if(Menu != nullptr)
    {
        Menu->RemoveMenu();
    }

    UEngine* Engine = GetEngine();
    if(!ensure(Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if(!ensure(PlayerController != nullptr)) return;

    UE_LOG(LogTemp, Warning, TEXT("Traveling to server..."));

    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);          // Travels to host's lobby.

    UE_LOG(LogTemp, Warning, TEXT("After traveling to server"));
}

// Set's gamemode of future game to game mode chosen by host.
void UPIXGameInstance::SetGameMode(const FString &GameMode) 
{
    SelectedGameMode = GameMode;
}

// Set's game mode ID to be used by health component to handle death events.
void UPIXGameInstance::SetGameModeID(const FString &GameModeID) 
{
    GMID = GameModeID;
}

// Load's map and game mode which correspond's to the chosen game mode.
void UPIXGameInstance::LoadGame() 
{
    if(LobbyMenu != nullptr)
    {
        LobbyMenu->RemoveMenu();
    }

    UWorld* World = GetWorld();
    if(!ensure(World != nullptr)) return;

   World->ServerTravel(SelectedGameMode);
}

// Loads main menu map.
void UPIXGameInstance::LoadMainMenu() 
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if(!ensure(PlayerController != nullptr)) return;

    PlayerController->ClientTravel("/Game/Maps/Intro", ETravelType::TRAVEL_Absolute);
}

void UPIXGameInstance::BackToLobby() 
{
    UWorld* World = GetWorld();
    if(!ensure(World != nullptr)) return;

    World->ServerTravel("/Game/Maps/Lobby?game=/Game/Blueprints/GameModes/BP_LobbyGameMode.BP_LobbyGameMode_C?listen");
}

void UPIXGameInstance::RemoveLobbyMenu() 
{
    if(LobbyMenu != nullptr)
    {
        LobbyMenu->RemoveMenu();
    }
}

void UPIXGameInstance::RemoveInGameMenu() 
{
    if(InGameMenu != nullptr)
    {
        InGameMenu->RemoveMenu();
    }
}

// "Replays" game by having the server re-travel to game mode map.
void UPIXGameInstance::Replay() 
{
    UWorld* World = GetWorld();
    if(!ensure(World != nullptr)) return;
    
    World->ServerTravel(SelectedGameMode);      // "Replays" by reloading/switching to level.
}

// Sets owning player's team.
void UPIXGameInstance::SetPlayerTeam(int Team) 
{
    PlayerTeam = Team;
}

// Sets owning player's unique specified name.
void UPIXGameInstance::SetPlayerName(FString Name) 
{
    PlayerName = Name;

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Player Name: %s"), *PlayerName));
}


// Return's active game mode ID.
FString UPIXGameInstance::GetGameModeID() 
{
    // For when not running game from startup. (Set appropriate GM ID).
    if(GMID == "") 
    { 
        return "FFA";       // Defaults to free for all     (FOR DEV ONLY!!!)
    }
    else
    {
        return GMID;
    }
}
