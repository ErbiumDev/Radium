#pragma once
float GVersion = 0.0f; //(TODO) Get the version somehow?
#include "Memory.h"
#include <string>
#include <locale>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
namespace Unreal {
	template<class T>
	struct TArray
	{
		friend class FString;

	public:
		inline TArray()
		{
			Data = nullptr;
			Count = Max = 0;
		};

		inline int Num() const
		{
			return Count;
		};

		inline void Add(T InputData)
		{
			Data = (T*)realloc(Data, sizeof(T) * (Count + 1));
			Data[Count++] = InputData;
			Max = Count;
		};

		inline void Remove(int32_t Index)
		{
			TArray<T> NewArray;
			for (int i = 0; i < this->Count; i++)
			{
				if (i == Index)
					continue;

				NewArray.Add(this->Data[i]);
			}
			this->Data = (T*)realloc(NewArray.Data, sizeof(T) * (NewArray.Count));
			this->Count = NewArray.Count;
			this->Max = NewArray.Count;
		}

		T* Data;
		int Count;
		int Max;
	};


	struct FString : private TArray<wchar_t>
	{
		FString()
		{
		};

		FString(const wchar_t* other)
		{
			Max = Count = *other ? std::wcslen(other) + 1 : 0;

			if (Count)
			{
				Data = const_cast<wchar_t*>(other);
			}
		}

		bool IsValid() const
		{
			return Data != nullptr;
		}

		const wchar_t* c_str() const
		{
			return Data;
		}

		std::string ToString() const
		{
			auto length = std::wcslen(Data);

			std::string str(length, '\0');

			std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

			return str;
		}
	};

	struct UObject;
	struct FName;
	//New
	FString (__fastcall* GetObjectFullName)(UObject* In);

	//Old
	void (__fastcall* FNameToString)(FName* NameIn, FString& Out);
	void(__fastcall* FnFree)(__int64);

	struct FName
	{
		uint32_t ComparisonIndex;
		uint32_t DisplayIndex;

		FName() = default;

		std::string GetName() {
			FString temp;
			FNameToString(this, temp);
			std::string ret(temp.ToString());
			FnFree(__int64(temp.c_str()));

			return ret;
		}
	};

	void* (__fastcall* ProcessEventOG)(void* Object, void* Function, void* Params);

	struct UObject
	{
		void** VTable;
		int32_t ObjectFlags;
		int32_t InternalIndex;
		UObject* Class;
		FName Name;
		UObject* Outer;

		uintptr_t GetAddress() {
			return __int64(this);
		}

		bool IsA(UObject* cmp) const
		{
			if (cmp == Class)
				return false;
			return false;
		}

		void* ProcessEvent(UObject* Function, void* Params = nullptr) {
			return ProcessEventOG(this, Function, Params);
		}

		std::string GetName() {
			return GetObjectFullName(this).ToString();
		}

		std::string GetNameOld() {
			return Name.GetName();
		}

		//Use GetName Instead!
		std::string GetFullName() {
			std::string temp;

			for (auto outer = Outer; outer; outer = outer->Outer)
			{
				temp = outer->Name.GetName() + "." + temp;
			}

			temp = reinterpret_cast<UObject*>(Class)->Name.GetName() + " " + temp + this->Name.GetName();

			return temp;
		}
	};

	struct FGuid
	{
		int A;
		int B;
		int C;
		int D;
	};

	struct UObjectItem
	{
		UObject* Object;
		DWORD Flags;
		DWORD ClusterIndex;
		DWORD SerialNumber;
	};

	struct PreUObjectItem
	{
		UObjectItem* FUObject[10];
	};

	class NewUObjectArray {
	public:
		UObjectItem* Objects[9];
	};

	struct GObjects
	{
		NewUObjectArray* ObjectArray;
		BYTE _padding_0[0xC];
		uint32_t NumElements;

		inline void NumChunks(int* start, int* end) const
		{
			int cStart = 0, cEnd = 0;

			if (!cEnd)
			{
				while (1)
				{
					if (ObjectArray->Objects[cStart] == 0)
					{
						cStart++;
					}
					else
					{
						break;
					}
				}

				cEnd = cStart;
				while (1)
				{
					if (ObjectArray->Objects[cEnd] == 0)
					{
						break;
					}
					else
					{
						cEnd++;
					}
				}
			}

			*start = cStart;
			*end = cEnd;
		}

		inline int32_t Num() const
		{
			return NumElements;
		}

		inline UObject* GetByIndex(int32_t index) const
		{
			int cStart = 0, cEnd = 0;
			int chunkIndex = 0, chunkSize = 0xFFFF, chunkPos;
			UObjectItem* Object;

			NumChunks(&cStart, &cEnd);

			chunkIndex = index / chunkSize;
			if (chunkSize * chunkIndex != 0 &&
				chunkSize * chunkIndex == index)
			{
				chunkIndex--;
			}

			chunkPos = cStart + chunkIndex;
			if (chunkPos < cEnd)
			{
				Object = ObjectArray->Objects[chunkPos] + (index - chunkSize * chunkIndex);
				return Object->Object;
			}

			return nullptr;
		}
	};

	class UObjectArray {
	public:
		inline int Num() const
		{
			return NumElements;
		}

		inline UObject* GetByIndex(int32_t index) const
		{
			return (&Objects[index])->Object;
		}

	private:
		UObjectItem* Objects;
		int MaxElements;
		int NumElements;
	};

	struct FVector {
		float X;
		float Y;
		float Z;

		FVector() {
			X = Y = Z = 0;
		}

		FVector(float NX, float NY, float NZ) {
			X = NX;
			Y = NY;
			Z = NZ;
		}
	};

	template <class TEnum>
	class TEnumAsByte
	{
	public:
		TEnumAsByte()
		{
		}

		TEnumAsByte(TEnum _value)
			: value(static_cast<uint8_t>(_value))
		{
		}

		explicit TEnumAsByte(int32_t _value)
			: value(static_cast<uint8_t>(_value))
		{
		}

		explicit TEnumAsByte(uint8_t _value)
			: value(_value)
		{
		}

		operator TEnum() const
		{
			return static_cast<TEnum>(value);
		}

		TEnum GetValue() const
		{
			return static_cast<TEnum>(value);
		}

	private:
		uint8_t value;
	};
}

enum class EFortCustomPartType : uint8_t
{
	Head = 0,
	Body = 1,
	Hat = 2,
	Backpack = 3,
	Charm = 4,
	Face = 5,
	NumTypes = 6,
	EFortCustomPartType_MAX = 7
};

struct Pointer{
	Unreal::UObject* Value;
};

Unreal::UObject* (__fastcall* GFPC)(Unreal::UObject* World);

namespace Game {
	//Vars
	bool InGame = false;
	//Main
	Unreal::GObjects* GObjs;
	Unreal::UObjectArray* ObjObjects;
	Unreal::UObject** GWorld;

	//In Game
	Unreal::UObject* GPC;
	Unreal::UObject* GPawn;
	Unreal::UObject* CM;
}

struct BitField
{
	unsigned char A : 1;
	unsigned char B : 1;
	unsigned char C : 1;
	unsigned char D : 1;
	unsigned char E : 1;
	unsigned char F : 1;
	unsigned char G : 1;
	unsigned char H : 1;
};

enum class ESpawnActorCollisionHandlingMethod : uint8_t
{
	Undefined = 0,
	AlwaysSpawn = 1,
	AdjustIfPossibleButAlwaysSpawn = 2,
	AdjustIfPossibleButDontSpawnIfColliding = 3,
	DontSpawnIfColliding = 4,
	ESpawnActorCollisionHandlingMethod_MAX = 5
};

struct FActorSpawnParameters
{
	FActorSpawnParameters() : Name(), Template(nullptr), Owner(nullptr), Instigator(nullptr), OverrideLevel(nullptr), SpawnCollisionHandlingOverride(), bRemoteOwned(0), bNoFail(0),
		bDeferConstruction(0),
		bAllowDuringConstructionScript(0),
		NameMode(),
		ObjectFlags()
	{
	}
	;


	Unreal::FName Name;

	Unreal::UObject* Template;

	Unreal::UObject* Owner;

	Unreal::UObject* Instigator;

	Unreal::UObject* OverrideLevel;

	ESpawnActorCollisionHandlingMethod SpawnCollisionHandlingOverride;

private:

	uint8_t bRemoteOwned : 1;

public:

	bool IsRemoteOwned() const { return bRemoteOwned; }

	uint8_t bNoFail : 1;


	uint8_t bDeferConstruction : 1;

	uint8_t bAllowDuringConstructionScript : 1;


	enum class ESpawnActorNameMode : uint8_t
	{
		Required_Fatal,

		Required_ErrorAndReturnNull,

		Required_ReturnNull,

		Requested
	};


	ESpawnActorNameMode NameMode;

	uint8_t ObjectFlags;
};
Unreal::UObject* (__fastcall*StaticLoadObjectInternal)(Unreal::UObject*, Unreal::UObject*, const TCHAR*, const TCHAR*, uint32_t, Unreal::UObject*, bool);
Unreal::UObject* (__fastcall*StaticConstructObjectInternal)(void*, void*, void*, int, unsigned int, void*, bool, void*, bool);
Unreal::UObject* (__fastcall*SpawnActor)(Unreal::UObject* World, Unreal::UObject* Class, Unreal::FVector* Loc, Unreal::FVector* Rot, const FActorSpawnParameters& SpawnParameters);

Unreal::UObject* FindObject(std::string TargetName, bool Equals = false) {
	if (GVersion < 5.0f) {
		for (int i = 0; i < Game::ObjObjects->Num(); i++) {
			Unreal::UObject* Object = Game::ObjObjects->GetByIndex(i);
			std::string ObjName = Object->GetName();
			if (ObjName == TargetName && Equals == true) {
				return Object;
			}
			if (ObjName.find(TargetName) != std::string::npos && Equals == false) {
				return Object;
			}
		}
	}
	else {
		for (int i = 0; i < Game::GObjs->Num(); i++) {
			Unreal::UObject* Object = Game::GObjs->GetByIndex(i);
			std::string ObjName = Object->GetName();
			if (ObjName == TargetName && Equals == true) {
				return Object;
			}
			if (ObjName.find(TargetName) != std::string::npos && Equals == false) {
				return Object;
			}
		}
	}
	return nullptr;
}

void DumpObjects() {
	std::ofstream log("Objects.txt");
	if (GVersion < 5.0f) {
		for (int i = 0; i < Game::ObjObjects->Num(); i++) {
			Unreal::UObject* Object = Game::ObjObjects->GetByIndex(i);
			std::string ObjName = Object->GetName();
			std::string item = "\nName: " + ObjName;
			log << item;
		}
	}
	else {
		for (int i = 0; i < Game::GObjs->Num(); i++) {
			Unreal::UObject* Object = Game::GObjs->GetByIndex(i);
			std::string ObjName = Object->GetName();
			std::string item = "\nName: " + ObjName;
			log << item;
		}
	}
}