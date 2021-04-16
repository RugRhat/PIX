// Fill out your copyright notice in the Description page of Project Settings.


#include "PIXGameInstance.h"
#include "Engine/Engine.h"
#include "MainMenu.h"
#include "MenuWidget.h"
#include "UObject/ConstructorHelpers.h"


UPIXGameInstance::UPIXGameInstance(const FObjectInitializer &ObjectInitializer) 
{
    // Allows for more control of Menu ui from source code rather than blueprints.
    ConstructorHelpers::FClassFinder<UUserWidget>MenuBPClass(TEXT("/Game/UI/WBP_MainMenu"));
    if(!ensure(MenuBPClass.Class != nullptr)) return;

    MenuClass = MenuBPClass.Class;

    // // Allows for more control of Death Screen from source code rather than blueprints.
    // ConstructorHelpers::FClassFinder<UUserWidget>DeathBPClass(TEXT("/Game/UI/WBP_DeathScreen"));
    // if(!ensure(DeathBPClass.Class != nullptr)) return;

    // DeathScreenClass = DeathBPClass.Class;

    // // Allows for more control of Respawn Screen from source code rather than blueprints.
    // ConstructorHelpers::FClassFinder<UUserWidget>RespawnBPClass(TEXT("/Game/UI/WBP_RespawnScreen"));
    // if(!ensure(RespawnBPClass.Class != nullptr)) return;

    // RespawnScreenClass = RespawnBPClass.Class;

    // // Allows for more control of Spectate Screen from source code rather than blueprints.
    // ConstructorHelpers::FClassFinder<UUserWidget>SpectateBPClass(TEXT("/Game/UI/WBP_SpectateScreen"));
    // if(!ensure(SpectateBPClass.Class != nullptr)) return;

    // SpectateScreenClass = SpectateBPClass.Class;

    // Allows for more control of Win Screen ui from source code rather than blueprints.
    // ConstructorHelpers::FClassFinder<UUserWidget>WinScreenMenuBPClass(TEXT("/Game/Blueprints/UI/WBP_WinScreen"));
    // if(!ensure(WinScreenMenuBPClass.Class != nullptr)) return;

    // WinScreenMenuClass = WinScreenMenuBPClass.Class;
}

// Sets up custom game instance properties.
void UPIXGameInstance::Init() 
{
    UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *MenuClass->GetName());
}

void UPIXGameInstance::LoadMenu() 
{
    if(!ensure(MenuClass != nullptr)) return;

    Menu = CreateWidget<UMainMenu>(this, MenuClass);

    Menu->Setup();

    Menu->SetMenuInterface(this);
}

// void UPIXGameInstance::LoadWinScreen() 
// {
    
// }

void UPIXGameInstance::Single() 
{
    UWorld* World = GetWorld();
    if(!ensure(World != nullptr)) return;

    World->ServerTravel("/Game/Maps/Horde?listen");      // Switches to Horde map. 
}

// Creates console command "Host".
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

    World->ServerTravel("/Game/Maps/Lobby?listen");      // Switches to Lobby map.

    Single();
}

// Creates console command "Join".
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

    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);          // Travels to host's lobby.
}

void UPIXGameInstance::SetGameMode(const FString &GameMode) 
{
    if(GameMode == "Horde")
    {
        ///TODO: SelectedGame = Horde file path.
    }
    else if(GameMode == "FFA")
    {
        ///TODO: SelectedGame = FFA file path.
    }
    else if(GameMode == "DM")
    {
        ///TODO: SelectedGame = DM file path.
    }

    ///TODO: Add the "? listen" jawn.
}

void UPIXGameInstance::LoadGame() 
{
    UWorld* World = GetWorld();
    if(!ensure(World != nullptr)) return;

    World->ServerTravel(SelectedGame);
}

// Loads Main Menu level.
void UPIXGameInstance::LoadMainMenu() 
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if(!ensure(PlayerController != nullptr)) return;

    PlayerController->ClientTravel("/Game/Maps/Intro", ETravelType::TRAVEL_Absolute);
}

void UPIXGameInstance::Replay() 
{
    UWorld* World = GetWorld();
    if(!ensure(World != nullptr)) return;

    World->ServerTravel(SelectedGame);      // "Replays" by reloading/switching to level.
}

// void UPIXGameInstance::LoadDeathScreen() 
// {
//     if(!ensure(DeathScreenClass != nullptr)) return;

//     UMenuWidget* DeathScreen = CreateWidget<UMenuWidget>(this, DeathScreenClass);

//     DeathScreen->Setup();

//     DeathScreen->SetMenuInterface(this);
// }

// void UPIXGameInstance::LoadRespawnScreen() 
// {
//     if(!ensure(RespawnScreenClass != nullptr)) return;

//     UMenuWidget* RespawnScreen = CreateWidget<UMenuWidget>(this, RespawnScreenClass);

//     RespawnScreen->Setup();

//     RespawnScreen->SetMenuInterface(this);
// }

// void UPIXGameInstance::LoadSpectateScreen() 
// {
//     if(!ensure(SpectateScreenClass != nullptr)) return;

//     UMenuWidget* SpectateScreen = CreateWidget<UMenuWidget>(this, SpectateScreenClass);

//     SpectateScreen->Setup();

//     SpectateScreen->SetMenuInterface(this);
// }


