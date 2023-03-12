#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <thread>
#include <cstdint>
#include <filesystem>

auto __stdcall entrypoint(HMODULE instance) -> void
{
    try
    {
        while (!GetAsyncKeyState(VK_DELETE))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    catch (const std::exception& error)
    {
        MessageBeep(MB_ICONERROR);
        MessageBox(nullptr, error.what(), "error in dll entrypoint", MB_OK | MB_ICONEXCLAMATION);
    }

    FreeLibraryAndExitThread(instance, 0);
}

int __stdcall DllMain(HMODULE instance, const std::uintptr_t& reason, const void* reserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(instance);

        const auto thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(entrypoint), instance, 0, nullptr);

        if (!thread)
        {
            return FALSE;
        }

        CloseHandle(thread);

        break;
    }
    default:
        break;
    }

    return TRUE;
}