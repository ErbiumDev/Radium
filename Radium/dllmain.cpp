#include "Hooks.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        AllocConsole();
        FILE* pFile;
        freopen_s(&pFile, ("CONOUT$"), "w", stdout);
        MH_Initialize();
        Init();
        uintptr_t ProcessEvent_Addr = __int64(Unreal::ProcessEventOG);
        MH_CreateHook((LPVOID)ProcessEvent_Addr, Hooks::ProcessEventHook, (LPVOID*)&Unreal::ProcessEventOG);
        MH_EnableHook((LPVOID)ProcessEvent_Addr);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

