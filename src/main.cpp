#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <thread>
#include <cstdint>
#include <filesystem>

#include "console/console.hpp"

auto __stdcall on_attach(LPVOID base) -> DWORD
{
#ifdef _DEBUG
    Console::get().attach();
#endif

    try
    {
        Console::get().print("initializing...\n");

        while (!GetAsyncKeyState(VK_DELETE))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

		FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
    }
    catch (const std::exception& exception)
    {
        Console::get().print("error occured during initalization:\n");
        Console::get().print("%s\n", exception.what());
        Console::get().detach();

		FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
    }
}

auto __stdcall on_detach() -> int
{
#ifdef _DEBUG
    Console::get().detach();
#endif

    return TRUE;
}

auto __stdcall DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) -> int
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(instance);
        CreateThread(nullptr, 0, on_attach, instance, 0, nullptr);

        return TRUE;
    }
    case DLL_PROCESS_DETACH:
    {
        if (reserved == nullptr)
        {
            return on_detach();
        }

        return TRUE;
    }
    default:
    {
        return TRUE;
    }
    }
}