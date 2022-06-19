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
        const char* ascii = (R"(


 .----------------. .----------------. .----------------. .----------------. .----------------. .----------------. 
| .--------------. | .--------------. | .--------------. | .--------------. | .--------------. | .--------------. |
| |  _______     | | |      __      | | |  ________    | | |     _____    | | | _____  _____ | | | ____    ____ | |
| | |_   __ \    | | |     /  \     | | | |_   ___ `.  | | |    |_   _|   | | ||_   _||_   _|| | ||_   \  /   _|| |
| |   | |__) |   | | |    / /\ \    | | |   | |   `. \ | | |      | |     | | |  | |    | |  | | |  |   \/   |  | |
| |   |  __ /    | | |   / ____ \   | | |   | |    | | | | |      | |     | | |  | '    ' |  | | |  | |\  /| |  | |
| |  _| |  \ \_  | | | _/ /    \ \_ | | |  _| |___.' / | | |     _| |_    | | |   \ `--' /   | | | _| |_\/_| |_ | |
| | |____| |___| | | ||____|  |____|| | | |________.'  | | |    |_____|   | | |    `.__.'    | | ||_____||_____|| |
| |              | | |              | | |              | | |              | | |              | | |              | |
| '--------------' | '--------------' | '--------------' | '--------------' | '--------------' | '--------------' |
 '----------------' '----------------' '----------------' '----------------' '----------------' '----------------' 
)");
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        std::cout << "\n" << ascii << "\n" << "By GD" << "\n";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        GVersion = Functions::GetVersion();
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

