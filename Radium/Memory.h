#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>

namespace Sigs {
	const char* GetFirstPlayerController = "83 B9 ? ? ? ? ? 7E ? 48 8B 89 ? ? ? ? E9";
	namespace UE416 {
		const char* StaticLoadObject = "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 8B 85 ? ? ? ? 33 F6 4C 8B BD ? ? ? ? 49 8B F9";
		const char* Free = "48 85 C9 74 2E 53 48 83 EC 20 48 8B D9 48 8B 0D ? ? ? ? 48 85 C9 75 0C";
		const char* FNameToString = "48 89 5C 24 ? 57 48 83 EC 40 83 79 04 00 48 8B DA 48 8B F9";
		const char* GetFullName = "40 53 48 83 EC 20 48 8B C2 48 8B D9 48 85 D2 75 2E";
		const char* GObjects = "48 8D 0D ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 48 8B D6"; //3
		const char* GWorld = "48 8B 1D ? ? ? ? ? ? ? 10 4C 8D 4D ? 4C"; //3
		const char* ProcessEvent = "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 48 63 41 0C";
		const char* StaticConstructObject = "4C 89 44 24 ? 53 55 56 57 41 54 41 56 41 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ?";
		const char* SpawnActor = "40 53 56 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 0F 28 1D ? ? ? ? 0F 57 D2 48 8B B4 24 ? ? ? ? 0F 28 CB";
	}

	namespace UE419 {
		const char* GWorld = "48 89 35 ? ? ? ? 41 8B 96 ? ? ? ?"; //3
	}

	namespace UE420 {
		const char* StaticLoadObjectOld = "4C 89 4C 24 ? 48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 48 8B EC";
		const char* StaticLoadObject = "4C 89 4C 24 ? 48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 33 D2";
		const char* Free = "48 85 C9 74 1D 4C 8B 05 ? ? ? ? 4D 85 C0";
		const char* FNameToString = "48 89 5C 24 ? 57 48 83 EC 40 83 79 04 00 48 8B DA 48 8B F9";
		const char* GetFullName = "40 53 48 83 EC 20 48 8B C2 48 8B D9 48 85 D2 75 2E";
		const char* GObjects = "48 8B 05 ? ? ? ? 48 8D 1C C8 81 4B ? ? ? ? ? 49 63 76 30"; //3
		const char* GWorld = "48 8B 1D ? ? ? ? 48 85 DB 74 3B 41"; //3
		const char* ProcessEvent = "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 48 63 41 0C 45 33 F6 3B 05 ? ? ? ?";
		const char* StaticConstructObject = "4C 89 44 24 ? 53 55 56 57 41 54 41 56 41 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ?";
		const char* SpawnActor = "40 53 56 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 0F 28 1D ? ? ? ? 0F 57 D2 48 8B B4 24 ? ? ? ? 0F 28 CB";
	}

	namespace UE421 {
		const char* StaticLoadObject = "4C 89 4C 24 ? 48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 78 45 33 F6 48 8D 05 ? ? ? ?";
		const char* Free = "48 85 C9 74 2E 53";
		const char* FNameToString = "48 89 5C 24 ? 57 48 83 EC 30 83 79 04 00 48 8B DA 48 8B F9";
		const char* GetFullName = "40 53 48 83 EC 20 48 8B C2 48 8B D9 48 85 D2 75 2E";
		const char* GObjects = "49 63 C8 48 8D 14 40 48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1"; //10
		const char* GWorld = "48 8B 1D ? ? ? ? 48 85 DB 74 3B 41"; //3
		const char* ProcessEvent = "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 8B 41 0C 45 33 F6 3B 05 ? ? ? ? 4D 8B F8 48 8B F2 4C 8B E1 41 B8 ? ? ? ? 7D 2A";
		const char* StaticConstructObject = "48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 44 8B A5 ? ? ? ? 48 8D 05";
		const char* SpawnActor = "40 53 56 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 0F 28 1D ? ? ? ? 0F 57 D2 48 8B B4 24 ? ? ? ? 0F 28 CB";
	}

	namespace UE422 {
		const char* StaticLoadObject = "4C 89 4C 24 ? 48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 78 45 33 F6 48 8D 05 ? ? ? ?";
		const char* Free = "48 85 C9 74 2E 53";
		const char* FNameToString = "48 89 5C 24 ? 57 48 83 EC 30 83 79 04 00 48 8B DA 48 8B F9";
		const char* GetFullName = "40 53 48 83 EC 20 48 8B C2 48 8B D9 48 85 D2 75 2E";
		const char* GObjects = "49 63 C8 48 8D 14 40 48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1"; //10
		const char* GWorld = "48 8B 1D ? ? ? ? 48 85 DB 74 3B 41"; //3
		const char* ProcessEvent = "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 8B 41 0C 45 33 F6 3B 05 ? ? ? ? 4D 8B F8 48 8B F2 4C 8B E1 41 B8 ? ? ? ? 7D 2A";
		const char* StaticConstructObject = "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 44 8B A5 ? ? ? ? 48 8D 05 ? ? ? ?";
		const char* SpawnActor = "40 53 56 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 0F 28 1D ? ? ? ? 0F 57 D2 48 8B B4 24 ? ? ? ? 0F 28 CB";
	}

	namespace UE423 {
		const char* StaticLoadObject = "4C 89 4C 24 ? 48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 78 45 33 F6";
		const char* Free = "48 85 C9 74 2E 53";
		const char* FNameToString = "48 89 5C 24 ? 57 48 83 EC 30 83 79 04 00 48 8B DA 48 8B F9";
		const char* GetFullName = "40 53 48 83 EC 20 48 8B C2 48 8B D9 48 85 D2 75 2E";
		const char* GObjects = "49 63 C8 48 8D 14 40 48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1"; //10
		const char* GWorld = "48 8B 1D ? ? ? ? 48 85 DB 74 3B 41"; //3
		const char* ProcessEvent = "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 8B 41 0C 45 33 F6 3B 05 ? ? ? ? 4D 8B F8 48 8B F2 4C 8B E1 41 B8 ? ? ? ? 7D 2A";
		const char* StaticConstructObject = "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 44 8B A5 ? ? ? ? 48 8D 05 ? ? ? ?";
		const char* SpawnActor = "40 53 56 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 0F 28 1D ? ? ? ? 0F 57 D2 48 8B B4 24 ? ? ? ? 0F 28 CB";
	}

	namespace UE424 {
		const char* GetFullName = "";
		const char* GObjects = "";
		const char* GWorld = "";
		const char* ProcessEvent = "";
		const char* StaticConstructObject = "";
		const char* SpawnActor = "";
	}

	namespace UE426 {
		const char* StaticLoadObject = "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 8B 85 ? ? ? ?";
		const char* Free = "48 85 C9 74 2E 53";
		const char* FNameToString = "48 89 5C 24 ? 55 56 57 48 8B EC 48 83 EC 30 8B 01 48 8B F1 44 8B 49 04 8B F8";
		const char* GetFullName = "40 53 48 83 EC 20 48 8B C2 48 8B D9 48 85 D2 75 43";
		const char* GObjects = "49 63 C8 48 8D 14 40 48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1"; //10
		const char* GWorld = "48 89 05 ? ? ? ? 48 8B B6 ? ? ? ?"; //3
		const char* ProcessEvent = "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 8B 41 0C 45 33 F6 3B 05 ? ? ? ? 4D 8B F8 48 8B F2 4C 8B E1 41 B8 ? ? ? ? 7D 2A";
		const char* StaticConstructObject = "4C 89 44 24 18 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4";
		const char* SpawnActor = "53 56 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 60 0F 28 1D ? ? ? ? 0F 57 D2 48 8B B4 24 B0";
	}

	namespace UE427 {
		//(TODO)
		const char* GetFullName = "";
		const char* GObjects = "";
		const char* GWorld = "";
		const char* ProcessEvent = "";
		const char* StaticConstructObject = "";
		const char* SpawnActor = "";
	}

	namespace UE51 {
		//(TODO)
		const char* GetFullName = "";
		const char* GObjects = "";
		const char* GWorld = "";
		const char* ProcessEvent = "";
		const char* StaticConstructObject = "";
		const char* SpawnActor = "";
	}
}

namespace Memory {
	//SigScan (not mine)
	uintptr_t SigScan(const char* signature, bool bRelative = false, uint32_t offset = 0) {
		uintptr_t base_address = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
		static auto patternToByte = [](const char* pattern)
		{
			auto bytes = std::vector<int>{};
			const auto start = const_cast<char*>(pattern);
			const auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;
					if (*current == '?') ++current;
					bytes.push_back(-1);
				}
				else { bytes.push_back(strtoul(current, &current, 16)); }
			}
			return bytes;
		};

		const auto dosHeader = (PIMAGE_DOS_HEADER)base_address;
		const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)base_address + dosHeader->e_lfanew);

		const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto patternBytes = patternToByte(signature);
		const auto scanBytes = reinterpret_cast<std::uint8_t*>(base_address);

		const auto s = patternBytes.size();
		const auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i)
		{
			bool found = true;
			for (auto j = 0ul; j < s; ++j)
			{
				if (scanBytes[i + j] != d[j] && d[j] != -1)
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				uintptr_t address = reinterpret_cast<uintptr_t>(&scanBytes[i]);
				if (bRelative)
				{
					address = ((address + offset + 4) + *(int32_t*)(address + offset));
					return address;
				}
				return address;
			}
		}
		return 0;
	}

	uintptr_t GetAddressFromOffset(DWORD Offset) {
		return reinterpret_cast<uintptr_t>(GetModuleHandle(NULL) + Offset);
	}

	uintptr_t GetAddressFromSig(const char* Sig, const char* Name)
	{
		if (Sig == "")
			return 0;
		std::cout << "\nFinding Address for " << Name << "!" << std::endl;
		uintptr_t Addr = SigScan(Sig);
		return Addr;
	}

	uintptr_t GetAddressFromSigR(const char* Sig, const char* Name, uint32_t ROffset)
	{
		if (Sig == "")
			return 0;
		std::cout << "\nFinding Address for " << Name << "!" << std::endl;
		uintptr_t Addr = SigScan(Sig, true, ROffset);
		return Addr;
	}
}