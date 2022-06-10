#pragma once
#include "Functions.h"

namespace Hooks {
	void* ProcessEventHook(Unreal::UObject* Object, Unreal::UObject* Function, void* Params) {
		std::string FuncName = Function->GetName();
		if (FuncName.find("BndEvt__PlayButton") != std::string::npos && Game::InGame == false) {
			Functions::PlayerController::UpdatePC();
			if (GVersion < 11.0f) {
				Functions::PlayerController::SwitchLevel(L"Athena_Terrain");
			}
			else {
				Functions::PlayerController::SwitchLevel(L"Apollo_Terrain");
			}
		}

		if (FuncName == "/Script/Engine.GameMode:ReadyToStartMatch" && Game::InGame == false) {
			DumpObjects();
			Game::InGame = true;
			int Season = Functions::GameUtils::GetSeason();
			Functions::PlayerController::UpdatePC();
			Functions::Player::InitCM();
			Unreal::FString PlrPawnClass = L"PlayerPawn_Athena_C";
			Game::CM->ProcessEvent(FindObject("/Script/Engine.CheatManager:Summon"), &PlrPawnClass);
			Game::GPawn = Finder::FindActor(FindObject("/Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C",true));
			Functions::PlayerController::Possess(Game::GPawn);
			Functions::Player::FixSpawnLoc();
			Unreal::UObject* GameState = *Finder::Find(*Game::GWorld, "GameState");
			uint8_t* GamePhase = Finder::Find<uint8_t*>(GameState, "GamePhase");
			*GamePhase = 2;
			Functions::GameState::OnRepGamePhase(GameState);
			if (Season != 4 && Season != 6 && Season != 7) {
				Unreal::UObject* GameMode = *Finder::Find(*Game::GWorld, "AuthorityGameMode");
				GameMode->ProcessEvent(FindObject("/Script/Engine.GameMode:StartMatch"));
			}
			Functions::PlayerController::ServerReadyToStartMatch();
			Functions::Player::ShowSkin();
			if (GVersion > 2.5f) {
				Functions::GameState::SetPlaylist(GameState, FindObject("/Game/Athena/Playlists/Playground/Playlist_Playground.Playlist_Playground"));
				Unreal::UObject* TargetClass = FindObject("/Script/FortniteGame.FortHLODSMActor");
				Game::CM->ProcessEvent(FindObject("/Script/Engine.CheatManager:DestroyAll"), &TargetClass);
			}

		}

		if (FuncName == "/Script/FortniteGame.FortPlayerControllerAthena:ServerAttemptAircraftJump") {
			Unreal::FString PlrPawnClass = L"PlayerPawn_Athena_C";
			Game::CM->ProcessEvent(FindObject("/Script/Engine.CheatManager:Summon"), &PlrPawnClass);
			Game::GPawn = Finder::FindActor(FindObject("/Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C",true));
			Unreal::FVector Loc;
			Game::GPC->ProcessEvent(FindObject("/Script/Engine.Actor:K2_GetActorLocation"), &Loc);
			struct {
				float X;
				float Y;
				float Z;
				float Pitch;
				float Yaw;
				float Roll;
			} params;
			params.X = Loc.X;
			params.Y = Loc.Y;
			params.Z = Loc.Z;
			params.Pitch = params.Yaw = params.Roll = 0;
			Game::CM->ProcessEvent(FindObject("/Script/Engine.CheatManager:BugItGo"), &params);
			Functions::PlayerController::Possess(Game::GPawn);
			Functions::Player::ShowSkin();
		}

		if (FuncName == "/Script/FortniteGame.FortPlayerController:ServerLoadingScreenDropped") {
			if (GVersion >= 2.5f) {
				Functions::Pawn::EquipGA(FindObject("/Script/FortniteGame.FortGameplayAbility_Jump"));
				Functions::Pawn::EquipGA(FindObject("/Script/FortniteGame.FortGameplayAbility_Sprint"));
				Functions::Pawn::EquipGA(FindObject("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GA_DefaultPlayer_InteractSearch.GA_DefaultPlayer_InteractSearch_C"));
				Functions::Pawn::EquipGA(FindObject("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GA_DefaultPlayer_InteractUse.GA_DefaultPlayer_InteractUse_C"));
				//Vehicle Stuff
				Functions::Pawn::EquipGA(FindObject("/Game/Athena/DrivableVehicles/GA_AthenaEnterVehicle.GA_AthenaEnterVehicle_C"));
				Functions::Pawn::EquipGA(FindObject("/Game/Athena/DrivableVehicles/GA_AthenaExitVehicle.GA_AthenaExitVehicle_C"));
				Functions::Pawn::EquipGA(FindObject("/Game/Athena/DrivableVehicles/GA_AthenaInVehicle.GA_AthenaInVehicle_C"));
			}
			if (GVersion >= 11.0f) {
				//Hiding Stuff
				Functions::Pawn::EquipGA(FindObject("/Game/Athena/Items/EnvironmentalItems/HidingProps/GA_Athena_HidingProp_JumpOut.GA_Athena_HidingProp_JumpOut_C"));
				Functions::Pawn::EquipGA(FindObject("/Game/Athena/Items/EnvironmentalItems/HidingProps/GA_Athena_HidingProp_Hide.GA_Athena_HidingProp_Hide_C"));
			}
			if (GVersion >= 19.0f) {
				//Sliding
				Functions::Pawn::EquipGA(FindObject("/Game/Abilities/Player/Sliding/GA_Athena_Player_Slide.GA_Athena_Player_Slide_C"));
			}
			Functions::Player::InitCM();
			//Inventory
			Functions::Inventory::WorldInventory = *Finder::Find(Game::GPC, "WorldInventory");
			Functions::Inventory::FortInventory = Finder::Find<void*>(Functions::Inventory::WorldInventory, "Inventory");
			Unreal::UObject** QuickBarP = Finder::Find(Game::GPC, "QuickBars");
			if (QuickBarP == nullptr) {
				Functions::Inventory::Quickbars = *Finder::Find(Game::GPC, "ClientQuickBars");
			}
			else {
				Unreal::FString Class = L"FortQuickBars";
				Game::CM->ProcessEvent(FindObject("/Script/Engine.CheatManager:Summon"), &Class);
				Functions::Inventory::Quickbars = Finder::FindActor(FindObject("/Script/FortniteGame.FortQuickBars"));
				*QuickBarP = Functions::Inventory::Quickbars;
			}
			Functions::Inventory::Quickbars->ProcessEvent(FindObject("/Script/Engine.Actor:SetOwner"), &Game::GPC);

			Functions::Inventory::AddItemToInventory(FindObject("/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01"), 0, Functions::Inventory::EFortQuickBars::Primary, 1); //At some point ima try to get the players pickaxe
			//(TODO) Add Building Stuff
			Unreal::UObject* GameState = *Finder::Find(*Game::GWorld, "GameState");
			Functions::GameState::SetupUI(GameState);
		}

		//Item swapping
		if (FuncName == "/Script/FortniteGame.FortPlayerController:ServerExecuteInventoryItem") {
			Unreal::FGuid* item = reinterpret_cast<Unreal::FGuid*>(Params);
			Functions::Inventory::EquipInventoryItem(item);
		}

		if (FuncName == "/Script/FortniteGame.FortPlayerPawn:ServerHandlePickup") {
			struct SHPparams
			{
				Unreal::UObject* Pickup;
				float InFlyTime;
				Unreal::FVector InStartDirection;
				bool bPlayPickupSound;
			};

			SHPparams* pParams = (SHPparams*)Params;

			if (pParams->Pickup != nullptr) {
				Unreal::UObject* Item = *reinterpret_cast<Unreal::UObject**>(__int64(pParams->Pickup) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortPickup"),"PrimaryPickupItemEntry") + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemEntry",true),"ItemDefinition"));
				Functions::Inventory::AddItemToInventory(Item, 1, Functions::Inventory::EFortQuickBars::Primary);
			}
		}

		return Unreal::ProcessEventOG(Object, Function, Params);
	}
}