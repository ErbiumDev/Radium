#pragma once
#include "Unreal.h"
namespace Unreal {
	struct UField_Old : UObject
	{
		UField_Old* Next;
	};

	struct UField_New : UObject
	{
		UField_New* Next;
		void* UKD_0;
		void* UKD_1;
	};

	struct UStruct_Old : UField_Old
	{
		UStruct_Old* Super;
		UField_Old* Children;
		uint32_t Size;
		char PAD[0x44];
	};

	struct UStruct_New : UField_New
	{
		UStruct_New* Super;
		UField_New* Children;
		uint32_t Size;
		char PAD[0x44];
	};

	struct FField {
		void* VTable;
		void* Class;
		void* Owner;
		void* UKD_00;
		FField* Next;
		FName Name;
		uint8_t FlagsPrivate;
	};

	struct UStruct_New2 : UField_New
	{
		UStruct_New2* Super;
		UField_New* Children;
		FField* ChildProperties;
		int32_t Size;
		int32_t MinAlignment;
		TArray<uint8_t> Script;
		void* PropertyLink;
		void* RefLink;
		void* DestructorLink;
		void* PostConstructLink;
	};
}

namespace Finder {
	using namespace Unreal;
	UObject* FindChild(Unreal::UObject* InObject, std::string PropName) {
		if (GVersion < 5.0f) {
			UField_Old* Prop = reinterpret_cast<UStruct_Old*>(InObject)->Children;
			while (Prop)
			{
				if (Prop->Name.GetName() == PropName) {
					return Prop;
				}
				else {
					Prop = Prop->Next;
				}
			}
			return nullptr;
		}
		else if (GVersion < 12.0f) {
			UField_New* Prop = reinterpret_cast<UStruct_New*>(InObject)->Children;
			while (Prop)
			{
				if (Prop->Name.GetName() == PropName) {
					return Prop;
				}
				else {
					Prop = Prop->Next;
				}
			}
			return nullptr;
		}
		else {
			FField* Prop = reinterpret_cast<UStruct_New2*>(InObject)->ChildProperties;
			while (Prop)
			{
				if (Prop->Name.GetName() == PropName) {
					return (UObject*)Prop;
				}
				else {
					Prop = Prop->Next;
				}
			}
			return nullptr;
		}
	}

	int GetOffset(void* TargetProp) {
		if (GVersion < 12.0f) {
			return *reinterpret_cast<int*>(__int64(TargetProp) + 0x44);
		}
		else {
			return *reinterpret_cast<int*>(__int64(TargetProp) + 0x4C);
		}
	}

	int FindOffset(UObject* TargetObject, std::string TargetChildName) {
		UObject* Prop = nullptr;
		if (GVersion < 5.0f) {
			UStruct_Old* Class = (UStruct_Old*)TargetObject->Class;
			if (Class->Children) {
				Prop = FindChild(Class, TargetChildName);
			}
			if (Prop == nullptr) {
				UStruct_Old* Struct = reinterpret_cast<UStruct_Old*>(TargetObject->Class)->Super;
				while (Struct)
				{
					if (Struct->Children) {
						Prop = FindChild(Struct, TargetChildName);
						if (Prop != nullptr) {
							break;
						}
					}
					Struct = Struct->Super;
				}
			}
		}
		else if (GVersion < 12.0f) {
			UStruct_New* Class = (UStruct_New*)TargetObject->Class;
			if (Class->Children) {
				Prop = FindChild(Class, TargetChildName);
			}
			if (Prop == nullptr) {
				UStruct_New* Struct = reinterpret_cast<UStruct_New*>(TargetObject->Class)->Super;
				while (Struct)
				{
					if (Struct->Children) {
						Prop = FindChild(Struct, TargetChildName);
						if (Prop != nullptr) {
							break;
						}
					}
					Struct = Struct->Super;
				}
			}
		}
		else {
			UStruct_New2* Class = (UStruct_New2*)TargetObject->Class;
			if (Class->Children) {
				Prop = FindChild(Class, TargetChildName);
			}
			if (Prop == nullptr) {
				UStruct_New2* Struct = reinterpret_cast<UStruct_New2*>(TargetObject->Class)->Super;
				while (Struct)
				{
					if (Struct->Children) {
						Prop = FindChild(Struct, TargetChildName);
						if (Prop != nullptr) {
							break;
						}
					}
					Struct = Struct->Super;
				}
			}
		}
		if (Prop != nullptr) {
			return GetOffset(Prop);
		}
		else {
			return 0;
		}
	}

	template<class T>
	T GetValuePointer(UObject* Object, void* Prop) {
		return reinterpret_cast<T>(Object->GetAddress() + GetOffset(Prop));
	}

	//Doesnt work on structs only UObject Based Types and types like ints, bools, etc (i think)
	template<class T = Unreal::UObject**>
	T Find(UObject* TargetObject, std::string TargetChildName) {
		UObject* Prop = nullptr;
		if (GVersion < 5.0f) {
			UStruct_Old* Class = (UStruct_Old*)TargetObject->Class;
			if (Class->Children) {
				Prop = FindChild(Class, TargetChildName);
			}
			if (Prop == nullptr) {
				UStruct_Old* Struct = reinterpret_cast<UStruct_Old*>(TargetObject->Class)->Super;
				while (Struct)
				{
					if (Struct->Children) {
						Prop = FindChild(Struct, TargetChildName);
						if (Prop != nullptr) {
							break;
						}
					}
					Struct = Struct->Super;
				}
			}
		}
		else if (GVersion < 12.0f) {
			UStruct_New* Class = (UStruct_New*)TargetObject->Class;
			if (Class->Children) {
				Prop = FindChild(Class, TargetChildName);
			}
			if (Prop == nullptr) {
				UStruct_New* Struct = reinterpret_cast<UStruct_New*>(TargetObject->Class)->Super;
				while (Struct)
				{
					if (Struct->Children) {
						Prop = FindChild(Struct, TargetChildName);
						if (Prop != nullptr) {
							break;
						}
					}
					Struct = Struct->Super;
				}
			}
		}
		else {
			UStruct_New2* Class = (UStruct_New2*)TargetObject->Class;
			if (Class->ChildProperties) {
				Prop = FindChild(Class, TargetChildName);
			}
			if (Prop == nullptr) {
				UStruct_New2* Struct = reinterpret_cast<UStruct_New2*>(TargetObject->Class)->Super;
				while (Struct)
				{
					if (Struct->Children) {
						Prop = FindChild(Struct, TargetChildName);
						if (Prop != nullptr) {
							break;
						}
					}
					Struct = Struct->Super;
				}
			}
		}
		if (Prop != nullptr) {
			return GetValuePointer<T>(TargetObject, Prop);
		}
		else {
			return nullptr;
		}
	}

	UObject* FindActor(UObject* Class, int Index = 0) {
		struct {
			UObject* World;
			UObject* Class;
			TArray<UObject*> Actors;
		} Params;
		Params.World = *Game::GWorld;
		Params.Class = Class;
		FindObject("/Script/Engine.Default__GameplayStatics")->ProcessEvent(FindObject("/Script/Engine.GameplayStatics:GetAllActorsOfClass"), &Params);
		auto Actors = Params.Actors;
		return Actors.Data[Index];
	}
}