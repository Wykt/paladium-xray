#include <Windows.h>
#include "xray.hpp"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call != DLL_PROCESS_ATTACH) {
        return TRUE;
    }

    DisableThreadLibraryCalls(hModule);
    HANDLE thread_handle = CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(xray::initialize), hModule, 0, 0);

    if (thread_handle)
    {
        CloseHandle(thread_handle);
    }

    return TRUE;
}

