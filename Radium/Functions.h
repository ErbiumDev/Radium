#pragma once
#pragma comment(lib,"minhook/minhook.lib")
#include "Finder.h"
#include "minhook/MinHook.h"
Unreal::UObject* PrevWAC = nullptr;
namespace Functions {

	//Credit: Ultimanite (I'm to lazy to rewrite this/find another way to get Version)
	//(TODO) Rewrite this + find a way that doesnt require FindObject
	float GetVersion() {
		Unreal::UObject* Default__FortRuntimeOptions = FindObject("/Script/FortniteGame.Default__FortRuntimeOptions");
		Unreal::UObject* GetGameVersion = FindObject("/Script/FortniteGame.FortRuntimeOptions:GetGameVersion");

		if (GetGameVersion)
		{
			Unreal::FString GameVersion;
			Default__FortRuntimeOptions->ProcessEvent(GetGameVersion, &GameVersion);
			std::string FNVer = GameVersion.ToString();
			FNVer.erase(0, FNVer.find_last_of(("-"), FNVer.length() - 1) + 1);

			return std::stof(FNVer);
		}
		else {
			return 1.8f; //(TODO) Get Version for Versions under ~2.5 
		}
		return 0.0f;
	}

	namespace PlayerController {
		void Possess(Unreal::UObject* InPawn) {
			Game::GPC->ProcessEvent(FindObject("/Script/Engine.Controller:Possess"), &InPawn);
		}

		void UpdatePC() {
			Game::GPC = GFPC(*Game::GWorld);
		}

		void SwitchLevel(Unreal::FString Map) {
			Game::GPC->ProcessEvent(FindObject("/Script/Engine.PlayerController:SwitchLevel"), &Map);
		}

		void ServerReadyToStartMatch() {
			Game::GPC->ProcessEvent(FindObject("/Script/FortniteGame.FortPlayerController:ServerReadyToStartMatch"));
		}
	}

	namespace Pawn {
		void EquipGameplayEffect(Unreal::UObject* GE) {
			struct FGameplayEffectContextHandle
			{
				char UKD_00[0x30];
			};
			struct
			{
				Unreal::UObject* GameplayEffectClass;
				float Level;
				FGameplayEffectContextHandle EffectContext;
				void* Ret;
			} Params;
			Params.EffectContext = FGameplayEffectContextHandle();
			Params.GameplayEffectClass = GE;
			Params.Level = 1.0;
			Unreal::UObject* ABC = *Finder::Find(Game::GPawn, "AbilitySystemComponent");
			ABC->ProcessEvent(FindObject("/Script/GameplayAbilities.AbilitySystemComponent:BP_ApplyGameplayEffectToSelf"), &Params);
		}

		void EquipGA(Unreal::UObject* GA) {
			//Credit Ultimanite for this
			struct FGameplayAbilitySpecDef
			{
				Unreal::UObject* Ability;
				unsigned char Unk00[0x90];
			};

			Unreal::UObject* DefaultGameplayEffect = nullptr;
			if (GVersion >= 7.0f)
			{
				DefaultGameplayEffect = FindObject(("/Game/Athena/Items/Consumables/PurpleStuff/GE_Athena_PurpleStuff_Health.Default__GE_Athena_PurpleStuff_Health_C"));
			}
			else {
				DefaultGameplayEffect = FindObject(("/Game/Athena/Items/Consumables/PurpleStuff/GE_Athena_PurpleStuff.Default__GE_Athena_PurpleStuff_C"));
			}
			Unreal::TArray<FGameplayAbilitySpecDef>* GrantedGAs = reinterpret_cast<Unreal::TArray<FGameplayAbilitySpecDef>*>(__int64(DefaultGameplayEffect) + Finder::FindOffset(FindObject("/Script/GameplayAbilities.GameplayEffect"),"GrantedAbilities"));
			GrantedGAs->Data[0].Ability = GA;
			enum class EGameplayEffectDurationType : uint8_t
			{
				Instant, Infinite, HasDuration, EGameplayEffectDurationType_MAX
			};
			*reinterpret_cast<EGameplayEffectDurationType*>(__int64(DefaultGameplayEffect) + Finder::FindOffset(FindObject("/Script/GameplayAbilities.GameplayEffect"),"DurationPolicy")) = EGameplayEffectDurationType::Infinite;
			auto GameplayEffectClass = FindObject(("/Game/Athena/Items/Consumables/PurpleStuff/GE_Athena_PurpleStuff_Health.GE_Athena_PurpleStuff_Health_C"));

			if (!GameplayEffectClass)
			{
				GameplayEffectClass = FindObject(("/Game/Athena/Items/Consumables/PurpleStuff/GE_Athena_PurpleStuff.GE_Athena_PurpleStuff_C"));
			}
			EquipGameplayEffect(GameplayEffectClass);
		}

		void ServerChoosePart(Unreal::UObject* TargetPawn, Unreal::UObject* Part, EFortCustomPartType Index) {

			struct
			{
				Unreal::TEnumAsByte<EFortCustomPartType> Type;
				Unreal::UObject* ChosenCharacterPart;
			} Params;

			Params.Type = Index;
			Params.ChosenCharacterPart = Part;

			TargetPawn->ProcessEvent(FindObject("/Script/FortniteGame.FortPlayerPawn:ServerChoosePart"), &Params);
		}

		void EquipItem(Unreal::UObject* TargetPawn, Unreal::UObject* WeaponDef, Unreal::FGuid Guid) {
			Unreal::UObject* Func = FindObject("/Script/FortniteGame.FortPawn:EquipWeaponDefinition");
			struct {
				Unreal::UObject* Def;
				Unreal::FGuid Guid;
				Unreal::UObject* Ret; //Idk if this works lmao
			} params;
			if (WeaponDef->Class->GetNameOld().find("AthenaGadget") != std::string::npos) {
				if (GVersion < 6.0f) {
					Unreal::UObject* GA;
					WeaponDef->ProcessEvent(FindObject("/Script/FortniteGame.FortGadgetItemDefinition:GetGameplayAbility"), &GA);
					WeaponDef->ProcessEvent(FindObject("/Script/FortniteGame.FortGadgetItemDefinition:GetDecoItemDefinition"), &WeaponDef);
					if (GA != nullptr) {
						Functions::Pawn::EquipGA(GA);
					}
				}
			}
			params.Def = WeaponDef;
			params.Guid = Guid;

			TargetPawn->ProcessEvent(Func, &params);
		}

		/*void EquipItem(Unreal::UObject* Pawn, Unreal::UObject* WeaponDef, Unreal::FGuid) {
			if (PrevWAC != nullptr) {
				PrevWAC->ProcessEvent(FindObject("/Script/Engine.Actor:K2_DestroyActor"));
			}
			Unreal::UObject* WAC;
			WeaponDef->ProcessEvent(FindObject("/Script/FortniteGame.FortWeaponItemDefinition:GetWeaponActorClass"), &WAC);
			Unreal::FString ClassName = std::wstring(WAC->GetName().begin(), WAC->GetName().end()).c_str();
			Game::CM->ProcessEvent(FindObject("/Script/Engine.CheatManager:Summon"), &ClassName);
			WAC = Finder::FindActor(WAC);
			WAC->ProcessEvent(FindObject("/Script/Engine.Actor:SetOwner"), &Game::GPawn);
			auto WAC_WeaponDef = Finder::Find(WAC, "WeaponData");
			*WAC_WeaponDef = WeaponDef;
			auto WAC_Guid = Finder::Find<Unreal::FGuid*>(WAC, "ItemEntryGuid");
			WAC->ProcessEvent(FindObject("/Script/FortniteGame.FortWeapon:OnRep_ReplicatedWeaponData"));
			WAC->ProcessEvent(FindObject("/Script/FortniteGame.FortWeapon:ClientGivenTo"), &Game::GPawn);
			Game::GPawn->ProcessEvent(FindObject("/Script/FortniteGame.FortPawn:ClientInternalEquipWeapon"), &WAC);
			PrevWAC = WAC;
		}*/
	}

	namespace Libs {
	}

	namespace GameState {
		void OnRepGamePhase(Unreal::UObject* GS, int OldGP = 0) {
			GS->ProcessEvent(FindObject("/Script/FortniteGame.FortGameStateAthena:OnRep_GamePhase"), &OldGP);
		}

		void SetPlaylist(Unreal::UObject* GS, Unreal::UObject* Playlist) {
			if (GVersion < 2.5f) {
				return;
			}
			if (GVersion < 7.0f) {
				Unreal::UObject** CPD = Finder::Find(GS, "CurrentPlaylistData");
				*CPD = Playlist;
				GS->ProcessEvent(FindObject("/Script/FortniteGame.FortGameStateAthena:OnRep_CurrentPlaylistData"));
			}
			else {
				Unreal::UObject** BasePlaylist = reinterpret_cast<Unreal::UObject**>(Finder::Find(GS, "CurrentPlaylistInfo") + __int64(Finder::FindOffset(FindObject("/Script/FortniteGame.PlaylistPropertyArray"),"BasePlaylist")));
				*BasePlaylist = Playlist;
				GS->ProcessEvent(FindObject("/Script/FortniteGame.FortGameStateAthena:OnRep_CurrentPlaylistInfo"));
			}
		}

		void SetupUI(Unreal::UObject* GS) {
			struct Brush
			{
				unsigned char PH0[0x48];
				Unreal::UObject* Texture;
			};
			int* PlayersLeft = Finder::Find<int*>(GS, "PlayersLeft");
			*PlayersLeft = 1;
			//Idk why but this doesnt work.
			//Finder::Find<Brush*>(GS, "MinimapBackgroundImage")->Texture = StaticLoadObjectInternal(FindObject("/Script/Engine.Texture2D", true), nullptr, L"/Game/Athena/HUD/MiniMap/MiniMapAthena.MiniMapAthena", 0, 0, 0, 0);
		}
	}

	namespace Player {
		//WIP Do not use
		Unreal::UObject* GetPickaxeWID() {
			void* Loadout = Finder::Find<void*>(Game::GPC, "CosmeticLoadoutPC");
			//(TODO) Find a way to get pickaxe without the Loadout Struct.
		}

		/*void ShowSkin() {
			if (GVersion >= 10.0f) {
				auto fn = FindObject(("/Script/FortniteGame.FortKismetLibrary:UpdatePlayerCustomCharacterPartsVisualization"));
				auto FKL = FindObject("/Script/FortniteGame.Default__FortKismetLibrary");
				Unreal::UObject* PlayerState = *Finder::Find(Game::GPC, "PlayerState");
				FKL->ProcessEvent(fn, &PlayerState);
			}
			else {
				Unreal::UObject* FortHero = FindObject("/Engine/Transient.FortHero");
				auto CharParts = reinterpret_cast<Unreal::TArray<Unreal::UObject*>*>(FortHero->GetAddress() + Finder::FindOffset(FindObject("/Script/FortniteGame.FortHero:CharacterParts")));
				Pawn::ServerChoosePart(Game::GPawn, CharParts->Data[0], EFortCustomPartType::Body);
				Pawn::ServerChoosePart(Game::GPawn, CharParts->Data[1], EFortCustomPartType::Head);
				Pawn::ServerChoosePart(Game::GPawn, CharParts->Data[2], EFortCustomPartType::Hat);
				Unreal::UObject* PlayerState = *Finder::Find(Game::GPC, "PlayerState");
				PlayerState->ProcessEvent(FindObject("/Script/FortniteGame.FortPlayerState:OnRep_CharacterParts"));
			}
		}*/

		//Doesnt work on ~10.40+
		void ShowSkin() {
			if (GVersion < 11.0f) {
				Pawn::ServerChoosePart(Game::GPawn, StaticLoadObjectInternal(FindObject("/Script/FortniteGame.CustomCharacterPart", true), nullptr, L"/Game/Characters/CharacterParts/Male/Medium/Heads/M_Med_Soldier_Head_01.M_Med_Soldier_Head_01", 0, 0, 0, 0), EFortCustomPartType::Head);
				Pawn::ServerChoosePart(Game::GPawn, StaticLoadObjectInternal(FindObject("/Script/FortniteGame.CustomCharacterPart", true), nullptr, L"/Game/Characters/CharacterParts/Male/Medium/Bodies/M_Med_Soldier_01_Base.M_Med_Soldier_01_Base", 0, 0, 0, 0), EFortCustomPartType::Body);
				Unreal::UObject* PlayerState = *Finder::Find(Game::GPawn, "PlayerState");
				if (GVersion < 10.0f) {
					PlayerState->ProcessEvent(FindObject("/Script/FortniteGame.FortPlayerState:OnRep_CharacterParts"));
				}
				else {
					PlayerState->ProcessEvent(FindObject("/Script/FortniteGame.FortPlayerState:OnRep_CharacterData"));
				}
			}
			else {
				//(TODO) C2 Jonesy
			}
		}

		void FixSpawnLoc() {
			if (GVersion < 11.0f) { // Only do it for C1
				struct {
					float X;
					float Y;
					float Z;
					float Pitch;
					float Yaw;
					float Roll;
				} params;
				params.X = 1;
				params.Y = 1;
				params.Z = 10000;
				params.Pitch = params.Yaw = params.Roll = 0;
				Game::CM->ProcessEvent(FindObject("/Script/Engine.CheatManager:BugItGo"), &params);
			}
		}

		//Cheat Manager
		void InitCM() {
			Unreal::UObject* NewCM = StaticConstructObjectInternal(FindObject("/Script/Engine.CheatManager"), Game::GPC, 0, 0, 0, 0, 0, 0, 0);
			auto CM = Finder::Find(Game::GPC, "CheatManager");
			*CM = NewCM;
			Game::CM = NewCM;
		}
	}

	namespace GameUtils {
		int GetSeason() {
			return (int)GVersion;
		}

		Unreal::UObject* GetWorld() {
			Unreal::UObject* Engine = FindObject("/Engine/Transient.FortEngine", false);
			auto GVP = *Finder::Find(Engine, "GameViewport");
			return *Finder::Find(GVP, "World");
		}
	}

	namespace Inventory {
		//Credit to Ultimanite for almost all of this
		Unreal::UObject* Quickbars;
		Unreal::UObject* WorldInventory;
		void* FortInventory;
		enum class EFortQuickBars : uint8_t
		{
			Primary, Secondary, Max_None, EFortQuickBars_MAX
		};

		Unreal::UObject* CreateItem(Unreal::UObject* ItemDefinition, int Count)
		{
			auto CreateTemporaryItemInstanceBP = FindObject("/Script/FortniteGame.FortItemDefinition:CreateTemporaryItemInstanceBP");

			struct
			{
				int Count;
				int Level;
				Unreal::UObject* ReturnValue;
			} Params;

			Params.Count = Count;
			Params.Level = 1;

			Unreal::ProcessEventOG(ItemDefinition, CreateTemporaryItemInstanceBP, &Params);

			auto TemporaryItemInstance = Params.ReturnValue;
			TemporaryItemInstance->ProcessEvent(FindObject("/Script/FortniteGame.FortItem:SetOwningControllerForTemporaryItem"), &Game::GPC);


			/*int* CurrentCount = reinterpret_cast<int*>(__int64(reinterpret_cast<void*>(TemporaryItemInstance + __int64(Finder::FindOffset(FindObject("/Script/FortniteGame.FortWorldItem:ItemEntry"))))) + static_cast<__int64>(0x0C));
			*CurrentCount = Count;*/

			return TemporaryItemInstance;
		}

		void UpdateInventory() {
			auto HandleWorldInventoryLocalUpdate = FindObject("/Script/FortniteGame.FortPlayerController:HandleWorldInventoryLocalUpdate");
			auto HandleInventoryLocalUpdate = FindObject("/Script/FortniteGame.FortInventory:HandleInventoryLocalUpdate");
			auto OnRep_QuickBar = FindObject("/Script/FortniteGame.FortPlayerController:OnRep_QuickBar");

			Unreal::ProcessEventOG(WorldInventory, HandleInventoryLocalUpdate, nullptr);
			Unreal::ProcessEventOG(Game::GPC, HandleWorldInventoryLocalUpdate, nullptr);
			if (OnRep_QuickBar)
			{
				auto OnRep_SecondaryQuickBar = FindObject("/Script/FortniteGame.FortQuickBars:OnRep_SecondaryQuickBar");
				auto OnRep_PrimaryQuickBar = FindObject("/Script/FortniteGame.FortQuickBars:OnRep_PrimaryQuickBar");
				Unreal::ProcessEventOG(Game::GPC, OnRep_QuickBar, nullptr);
				Unreal::ProcessEventOG(Quickbars, OnRep_SecondaryQuickBar, nullptr);
				Unreal::ProcessEventOG(Quickbars, OnRep_PrimaryQuickBar, nullptr);
			}
			else
			{
				auto ClientForceUpdateQuickbar = FindObject("/Script/FortniteGame.FortPlayerController:ClientForceUpdateQuickbar");
				auto PrimaryQuickbar = EFortQuickBars::Primary;
				auto SecondaryQuickbar = EFortQuickBars::Secondary;
				Unreal::ProcessEventOG(Game::GPC, ClientForceUpdateQuickbar, &PrimaryQuickbar);
				Unreal::ProcessEventOG(Game::GPC, ClientForceUpdateQuickbar, &SecondaryQuickbar);
			}
		}

		bool GUIDsMatch(Unreal::FGuid A, Unreal::FGuid B) {
			return A.A == B.A && A.B == B.B && A.C == B.C && A.D == B.D;
		}

		void EquipInventoryItem(Unreal::FGuid* GUID) {
			int Version = GameUtils::GetSeason();
			if (Version == 1) {
				struct ItemEntrySize
				{
					unsigned char Unk00[0xA8];
				};
				auto Entries = reinterpret_cast<Unreal::TArray<ItemEntrySize>*>(__int64(FortInventory) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemList"),"ReplicatedEntries"));
				for (int i = 0; i < Entries->Num(); i++) {
					ItemEntrySize Entry = Entries->Data[i];
					Unreal::FGuid* ItemGUID = reinterpret_cast<Unreal::FGuid*>(__int64(&Entry) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemEntry",true),"ItemGuid"));
					if (GUIDsMatch(*GUID, *ItemGUID)) {
						Unreal::UObject* ItemDef = *reinterpret_cast<Unreal::UObject**>(__int64(&Entry) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemEntry",true),"ItemDefinition"));
						Pawn::EquipItem(Game::GPawn, ItemDef, *ItemGUID);
						return;
					}
				}
			}
			if (Version == 4 || Version == 5 || Version == 6) {
				struct ItemEntrySize
				{
					unsigned char Unk00[0xD0];
				};
				auto Entries = reinterpret_cast<Unreal::TArray<ItemEntrySize>*>(__int64(FortInventory) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemList"), "ReplicatedEntries"));
				for (int i = 0; i < Entries->Num(); i++) {
					ItemEntrySize Entry = Entries->Data[i];
					Unreal::FGuid* ItemGUID = reinterpret_cast<Unreal::FGuid*>(__int64(&Entry) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemEntry", true), "ItemGuid"));
					if (GUIDsMatch(*GUID, *ItemGUID)) {
						Unreal::UObject* ItemDef = *reinterpret_cast<Unreal::UObject**>(__int64(&Entry) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemEntry", true), "ItemDefinition"));
						Pawn::EquipItem(Game::GPawn, ItemDef, *ItemGUID);
						return;
					}
				}
			}
			if (Version == 7 || Version == 8 || Version == 9 || Version == 10) {
				struct ItemEntrySize
				{
					unsigned char Unk00[0x120];
				};
				auto Entries = reinterpret_cast<Unreal::TArray<ItemEntrySize>*>(__int64(FortInventory) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemList"), "ReplicatedEntries"));
				for (int i = 0; i < Entries->Num(); i++) {
					ItemEntrySize Entry = Entries->Data[i];
					Unreal::FGuid* ItemGUID = reinterpret_cast<Unreal::FGuid*>(__int64(&Entry) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemEntry", true), "ItemGuid"));
					if (GUIDsMatch(*GUID, *ItemGUID)) {
						Unreal::UObject* ItemDef = *reinterpret_cast<Unreal::UObject**>(__int64(&Entry) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemEntry", true), "ItemDefinition"));
						Pawn::EquipItem(Game::GPawn, ItemDef, *ItemGUID);
						return;
					}
				}
			}
		}

		void AddItemToInventory(Unreal::UObject* Item, int Slot, EFortQuickBars QBIndex, int Count = 1) {
			Item = CreateItem(Item, Count);
			int Version = GameUtils::GetSeason();
			if (Version == 1) {
				struct ItemEntrySize
				{
					unsigned char Unk00[0xA8];
				};
				auto ItemEntry = reinterpret_cast<ItemEntrySize*>(__int64(Item) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortWorldItem"), "ItemEntry"));
				reinterpret_cast<Unreal::TArray<ItemEntrySize>*>(__int64(FortInventory) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemList", true), "ReplicatedEntries"))->Add(*ItemEntry);
			}
			if (Version == 3) {
				struct ItemEntrySize
				{
					unsigned char Unk00[0xC0];
				};
				auto ItemEntry = reinterpret_cast<ItemEntrySize*>(__int64(Item) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortWorldItem"),"ItemEntry"));
				reinterpret_cast<Unreal::TArray<ItemEntrySize>*>(__int64(FortInventory) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemList",true),"ReplicatedEntries"))->Add(*ItemEntry);
			}
			if (Version == 4 || Version == 5 || Version == 6) {
				struct ItemEntrySize
				{
					unsigned char Unk00[0xD0];
				};
				auto ItemEntry = reinterpret_cast<ItemEntrySize*>(__int64(Item) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortWorldItem"), "ItemEntry"));
				reinterpret_cast<Unreal::TArray<ItemEntrySize>*>(__int64(FortInventory) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemList", true), "ReplicatedEntries"))->Add(*ItemEntry);
			}
			if (Version == 7 || Version == 8 || Version == 9 || Version == 10) {
				struct ItemEntrySize
				{
					unsigned char Unk00[0x120];
				};
				auto ItemEntry = reinterpret_cast<ItemEntrySize*>(__int64(Item) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortWorldItem"), "ItemEntry"));
				reinterpret_cast<Unreal::TArray<ItemEntrySize>*>(__int64(FortInventory) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemList", true), "ReplicatedEntries"))->Add(*ItemEntry);
			}
			if (Version == 13 || Version == 14) {
				struct ItemEntrySize
				{
					unsigned char Unk00[0x160];
				};
				auto ItemEntry = reinterpret_cast<ItemEntrySize*>(__int64(Item) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortWorldItem"), "ItemEntry"));
				reinterpret_cast<Unreal::TArray<ItemEntrySize>*>(__int64(FortInventory) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemList", true), "ReplicatedEntries"))->Add(*ItemEntry);
			}

			//I'll have to check if this is the version
			if (Version < 9.0f) {
				reinterpret_cast<Unreal::TArray<Unreal::UObject*>*>(__int64(FortInventory) + Finder::FindOffset(FindObject("/Script/FortniteGame.FortItemList"),"ItemInstances"))->Add(Item);
			}
			if (GVersion < 7.40f) {
				struct
				{
					Unreal::FGuid Item;
					EFortQuickBars Quickbar;
					int Slot;
				} Params;
				Item->ProcessEvent(FindObject("/Script/FortniteGame.FortItem:GetItemGuid"), &Params.Item);

				Params.Quickbar = QBIndex;
				Params.Slot = Slot;

				Quickbars->ProcessEvent(FindObject("/Script/FortniteGame.FortQuickBars:ServerAddItemInternal"), &Params);
			}
			UpdateInventory();
		}
	}
}

void Init() {
	GFPC = decltype(GFPC)(Memory::GetAddressFromSig(Sigs::GetFirstPlayerController, "GetFirstPlayerController"));
	if (GVersion < 2.5) {
		uintptr_t FNTS_Addr = Memory::GetAddressFromSig(Sigs::UE416::FNameToString, "FName::ToString 4.16");
		uintptr_t FnFree_Addr = Memory::GetAddressFromSig(Sigs::UE416::Free, "FnFree 4.16");
		uintptr_t ProcessEvent_Addr = Memory::GetAddressFromSig(Sigs::UE416::ProcessEvent, "UObject::ProcessEvent 4.16");
		uintptr_t GetFullName_Addr = Memory::GetAddressFromSig(Sigs::UE416::GetFullName, "UObject::GetFullName 4.16");
		uintptr_t GObjects_Addr = Memory::GetAddressFromSigR(Sigs::UE416::GObjects, "GUObjectArray 4.16", 3);
		uintptr_t GWorld_Addr = Memory::GetAddressFromSigR(Sigs::UE416::GWorld, "GWorld 4.16", 3);
		uintptr_t SCO_Addr = Memory::GetAddressFromSig(Sigs::UE416::StaticConstructObject, "StaticConstructObject 4.16");
		uintptr_t SLO_Addr = Memory::GetAddressFromSig(Sigs::UE416::StaticLoadObject, "StaticLoadObject 4.16");
		uintptr_t SA_Addr = Memory::GetAddressFromSig(Sigs::UE416::SpawnActor, "UWorld::SpawnActor 4.16");
		Unreal::GetObjectFullName = decltype(Unreal::GetObjectFullName)(GetFullName_Addr);
		Unreal::FNameToString = decltype(Unreal::FNameToString)(FNTS_Addr);
		Unreal::FnFree = decltype(Unreal::FnFree)(FnFree_Addr);
		class OAP {
		public:
			int32_t ObjFirstGCIndex;
			int32_t ObjLastNonGCIndex;
			int32_t MaxObjectsNotConsideredByGC;
			int32_t OpenForDisregardForGC;
			Unreal::UObjectArray Objects;
		};
		//Game::GObjs = new Unreal::UObjectArray(reinterpret_cast<OAP*>(GObjects_Addr)->Objects);
		Game::ObjObjects = new Unreal::UObjectArray(reinterpret_cast<OAP*>(GObjects_Addr)->Objects);
		Game::GWorld = reinterpret_cast<Unreal::UObject**>(GWorld_Addr);
		SpawnActor = decltype(SpawnActor)(SA_Addr);
		StaticConstructObjectInternal = decltype(StaticConstructObjectInternal)(SCO_Addr);
		StaticLoadObjectInternal = decltype(StaticLoadObjectInternal)(SLO_Addr);
		Unreal::ProcessEventOG = decltype(Unreal::ProcessEventOG)(ProcessEvent_Addr);
	}
	else if (GVersion < 5.0f) {
		uintptr_t FNTS_Addr = Memory::GetAddressFromSig(Sigs::UE420::FNameToString, "FName::ToString 4.20");
		uintptr_t FnFree_Addr = Memory::GetAddressFromSig(Sigs::UE420::Free, "FnFree 4.20");
		uintptr_t ProcessEvent_Addr = Memory::GetAddressFromSig(Sigs::UE420::ProcessEvent, "UObject::ProcessEvent 4.20");
		uintptr_t GetFullName_Addr = Memory::GetAddressFromSig(Sigs::UE420::GetFullName, "UObject::GetFullName 4.20");
		uintptr_t GObjects_Addr = Memory::GetAddressFromSigR(Sigs::UE420::GObjects, "GUObjectArray 4.20", 3);
		uintptr_t GWorld_Addr = Memory::GetAddressFromSigR(Sigs::UE420::GWorld, "GWorld 4.20", 3);
		uintptr_t SCO_Addr = Memory::GetAddressFromSig(Sigs::UE420::StaticConstructObject, "StaticConstructObject 4.20");
		uintptr_t SLO_Addr = Memory::GetAddressFromSig(Sigs::UE420::StaticLoadObject, "StaticLoadObject 4.20");
		uintptr_t SA_Addr = Memory::GetAddressFromSig(Sigs::UE420::SpawnActor, "UWorld::SpawnActor 4.20");
		Unreal::GetObjectFullName = decltype(Unreal::GetObjectFullName)(GetFullName_Addr);
		Unreal::FNameToString = decltype(Unreal::FNameToString)(FNTS_Addr);
		Unreal::FnFree = decltype(Unreal::FnFree)(FnFree_Addr);
		Game::GObjs = reinterpret_cast<Unreal::GObjects*>(GObjects_Addr);
		Game::ObjObjects = reinterpret_cast<Unreal::UObjectArray*>(GObjects_Addr);
		Game::GWorld = reinterpret_cast<Unreal::UObject**>(GWorld_Addr);
		SpawnActor = decltype(SpawnActor)(SA_Addr);
		StaticConstructObjectInternal = decltype(StaticConstructObjectInternal)(SCO_Addr);
		StaticLoadObjectInternal = decltype(StaticLoadObjectInternal)(SLO_Addr);
		Unreal::ProcessEventOG = decltype(Unreal::ProcessEventOG)(ProcessEvent_Addr);
	}
	else if (GVersion < 7.0f) {
		uintptr_t FNTS_Addr = Memory::GetAddressFromSig(Sigs::UE421::FNameToString, "FName::ToString 4.21");
		uintptr_t FnFree_Addr = Memory::GetAddressFromSig(Sigs::UE421::Free, "FnFree 4.21");
		uintptr_t ProcessEvent_Addr = Memory::GetAddressFromSig(Sigs::UE421::ProcessEvent, "UObject::ProcessEvent 4.21");
		uintptr_t GetFullName_Addr = Memory::GetAddressFromSig(Sigs::UE421::GetFullName, "UObject::GetFullName 4.21");
		uintptr_t GObjects_Addr = Memory::GetAddressFromSigR(Sigs::UE421::GObjects, "GUObjectArray 4.21", 10);
		uintptr_t GWorld_Addr = Memory::GetAddressFromSigR(Sigs::UE421::GWorld, "GWorld 4.21", 3);
		uintptr_t SCO_Addr = Memory::GetAddressFromSig(Sigs::UE421::StaticConstructObject, "StaticConstructObject 4.21");
		uintptr_t SLO_Addr = Memory::GetAddressFromSig(Sigs::UE421::StaticLoadObject, "StaticLoadObject 4.21");
		uintptr_t SA_Addr = Memory::GetAddressFromSig(Sigs::UE421::SpawnActor, "UWorld::SpawnActor 4.21");
		Unreal::GetObjectFullName = decltype(Unreal::GetObjectFullName)(GetFullName_Addr);
		Unreal::FNameToString = decltype(Unreal::FNameToString)(FNTS_Addr);
		Unreal::FnFree = decltype(Unreal::FnFree)(FnFree_Addr);
		Game::GObjs = reinterpret_cast<Unreal::GObjects*>(GObjects_Addr);
		Game::ObjObjects = reinterpret_cast<Unreal::UObjectArray*>(GObjects_Addr);
		Game::GWorld = reinterpret_cast<Unreal::UObject**>(GWorld_Addr);
		SpawnActor = decltype(SpawnActor)(SA_Addr);
		StaticConstructObjectInternal = decltype(StaticConstructObjectInternal)(SCO_Addr);
		StaticLoadObjectInternal = decltype(StaticLoadObjectInternal)(SLO_Addr);
		Unreal::ProcessEventOG = decltype(Unreal::ProcessEventOG)(ProcessEvent_Addr);
	}
	else if (GVersion < 8.0f) {
		uintptr_t FNTS_Addr = Memory::GetAddressFromSig(Sigs::UE422::FNameToString, "FName::ToString 4.22");
		uintptr_t FnFree_Addr = Memory::GetAddressFromSig(Sigs::UE422::Free, "FnFree 4.22");
		uintptr_t ProcessEvent_Addr = Memory::GetAddressFromSig(Sigs::UE422::ProcessEvent, "UObject::ProcessEvent 4.22");
		uintptr_t GetFullName_Addr = Memory::GetAddressFromSig(Sigs::UE422::GetFullName, "UObject::GetFullName 4.22");
		uintptr_t GObjects_Addr = Memory::GetAddressFromSigR(Sigs::UE422::GObjects, "GUObjectArray 4.22", 10);
		uintptr_t GWorld_Addr = Memory::GetAddressFromSigR(Sigs::UE422::GWorld, "GWorld 4.22", 3);
		uintptr_t SCO_Addr = Memory::GetAddressFromSig(Sigs::UE422::StaticConstructObject, "StaticConstructObject 4.22");
		uintptr_t SLO_Addr = Memory::GetAddressFromSig(Sigs::UE422::StaticLoadObject, "StaticLoadObject 4.22");
		uintptr_t SA_Addr = Memory::GetAddressFromSig(Sigs::UE422::SpawnActor, "UWorld::SpawnActor 4.22");
		Unreal::GetObjectFullName = decltype(Unreal::GetObjectFullName)(GetFullName_Addr);
		Unreal::FNameToString = decltype(Unreal::FNameToString)(FNTS_Addr);
		Unreal::FnFree = decltype(Unreal::FnFree)(FnFree_Addr);
		Game::GObjs = reinterpret_cast<Unreal::GObjects*>(GObjects_Addr);
		Game::ObjObjects = reinterpret_cast<Unreal::UObjectArray*>(GObjects_Addr);
		Game::GWorld = reinterpret_cast<Unreal::UObject**>(GWorld_Addr);
		SpawnActor = decltype(SpawnActor)(SA_Addr);
		StaticConstructObjectInternal = decltype(StaticConstructObjectInternal)(SCO_Addr);
		StaticLoadObjectInternal = decltype(StaticLoadObjectInternal)(SLO_Addr);
		Unreal::ProcessEventOG = decltype(Unreal::ProcessEventOG)(ProcessEvent_Addr);
	}
	else if (GVersion < 11.0f) {
		uintptr_t FNTS_Addr = Memory::GetAddressFromSig(Sigs::UE423::FNameToString, "FName::ToString 4.23");
		uintptr_t FnFree_Addr = Memory::GetAddressFromSig(Sigs::UE423::Free, "FnFree 4.23");
		uintptr_t ProcessEvent_Addr = Memory::GetAddressFromSig(Sigs::UE423::ProcessEvent, "UObject::ProcessEvent 4.23");
		uintptr_t GetFullName_Addr = Memory::GetAddressFromSig(Sigs::UE423::GetFullName, "UObject::GetFullName 4.23");
		uintptr_t GObjects_Addr = Memory::GetAddressFromSigR(Sigs::UE423::GObjects, "GUObjectArray 4.23", 10);
		uintptr_t GWorld_Addr = Memory::GetAddressFromSigR(Sigs::UE423::GWorld, "GWorld 4.23", 3);
		uintptr_t SCO_Addr = Memory::GetAddressFromSig(Sigs::UE423::StaticConstructObject, "StaticConstructObject 4.23");
		uintptr_t SLO_Addr = Memory::GetAddressFromSig(Sigs::UE423::StaticLoadObject, "StaticLoadObject 4.23");
		uintptr_t SA_Addr = Memory::GetAddressFromSig(Sigs::UE423::SpawnActor, "UWorld::SpawnActor 4.23");
		Unreal::GetObjectFullName = decltype(Unreal::GetObjectFullName)(GetFullName_Addr);
		Unreal::FNameToString = decltype(Unreal::FNameToString)(FNTS_Addr);
		Unreal::FnFree = decltype(Unreal::FnFree)(FnFree_Addr);
		Game::GObjs = reinterpret_cast<Unreal::GObjects*>(GObjects_Addr);
		Game::ObjObjects = reinterpret_cast<Unreal::UObjectArray*>(GObjects_Addr);
		Game::GWorld = reinterpret_cast<Unreal::UObject**>(GWorld_Addr);
		SpawnActor = decltype(SpawnActor)(SA_Addr);
		StaticConstructObjectInternal = decltype(StaticConstructObjectInternal)(SCO_Addr);
		StaticLoadObjectInternal = decltype(StaticLoadObjectInternal)(SLO_Addr);
		Unreal::ProcessEventOG = decltype(Unreal::ProcessEventOG)(ProcessEvent_Addr);
	}
	else if (GVersion < 16.0f) {
		uintptr_t FNTS_Addr = Memory::GetAddressFromSig(Sigs::UE426::FNameToString, "FName::ToString 4.26");
		uintptr_t FnFree_Addr = Memory::GetAddressFromSig(Sigs::UE426::Free, "FnFree 4.26");
		uintptr_t ProcessEvent_Addr = Memory::GetAddressFromSig(Sigs::UE426::ProcessEvent, "UObject::ProcessEvent 4.26");
		uintptr_t GetFullName_Addr = Memory::GetAddressFromSig(Sigs::UE426::GetFullName, "UObject::GetFullName 4.26");
		uintptr_t GObjects_Addr = Memory::GetAddressFromSigR(Sigs::UE426::GObjects, "GUObjectArray 4.26", 10);
		uintptr_t GWorld_Addr = Memory::GetAddressFromSigR(Sigs::UE426::GWorld, "GWorld 4.26", 3);
		if (GWorld_Addr == 0) {
			GWorld_Addr = Memory::GetAddressFromSigR(Sigs::UE423::GWorld, "GWorld 4.26 - Old", 3);
		}
		uintptr_t SCO_Addr = Memory::GetAddressFromSig(Sigs::UE426::StaticConstructObject, "StaticConstructObject 4.26");
		uintptr_t SLO_Addr = Memory::GetAddressFromSig(Sigs::UE426::StaticLoadObject, "StaticLoadObject 4.26");
		uintptr_t SA_Addr = Memory::GetAddressFromSig(Sigs::UE426::SpawnActor, "UWorld::SpawnActor 4.26");
		Unreal::GetObjectFullName = decltype(Unreal::GetObjectFullName)(GetFullName_Addr);
		Unreal::FNameToString = decltype(Unreal::FNameToString)(FNTS_Addr);
		Unreal::FnFree = decltype(Unreal::FnFree)(FnFree_Addr);
		Game::GObjs = reinterpret_cast<Unreal::GObjects*>(GObjects_Addr);
		Game::ObjObjects = reinterpret_cast<Unreal::UObjectArray*>(GObjects_Addr);
		Game::GWorld = reinterpret_cast<Unreal::UObject**>(GWorld_Addr);
		SpawnActor = decltype(SpawnActor)(SA_Addr);
		StaticConstructObjectInternal = decltype(StaticConstructObjectInternal)(SCO_Addr);
		StaticLoadObjectInternal = decltype(StaticLoadObjectInternal)(SLO_Addr);
		Unreal::ProcessEventOG = decltype(Unreal::ProcessEventOG)(ProcessEvent_Addr);
	}
}