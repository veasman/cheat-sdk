#include "hooks.hpp"
#include "../../common/console/console.hpp"

#include <intrin.h>
#include <stdexcept>

#include <minhook/MinHook.h>

#if _WIN64
#pragma comment(lib, "libMinHook.x64.lib")
#else
#pragma comment(lib, "libMinHook.x86.lib")
#endif

#ifdef _DEBUG
#define DEBUG_MSG_BOX(msg) MessageBoxA(NULL, (msg), "Error", MB_ICONERROR)
#else
#define DEBUG_MSG_BOX(msg)
#endif

Hooks::Hooks()
{

}

Hooks::~Hooks()
{

}

auto Hooks::setup() -> void
{
    // Initialize MinHook
    if (MH_Initialize() != MH_OK)
    {
        throw new std::runtime_error("Failed to initialize MinHook");
    }

    // Get a pointer to the EndScene function and hook it
    IDirect3D9Ex* device = nullptr;
    EndScene endScene = nullptr;

    if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &device)))
    {
        throw new std::runtime_error("Failed to create Direct3D9Ex device");
    }

    if (MH_CreateHook(reinterpret_cast<LPVOID>(endScene), virtual_function(end_scene, 42), reinterpret_cast<LPVOID*>(&end_scene_original)) != MH_OK)
    {
        throw new std::runtime_error("Failed to create EndScene hook");
    }

    if (MH_CreateHook(reinterpret_cast<LPVOID>(reset), virtual_function(reset, 16), reinterpret_cast<LPVOID*>(&reset_original)) != MH_OK)
    {
        throw new std::runtime_error("Failed to create reset hook");
    }

    // Enable all hooks
    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
    {
        throw new std::runtime_error("failed to enable all hooks");
    }
}

auto Hooks::destroy() -> void
{
    if (MH_RemoveHook(MH_ALL_HOOKS) != MH_OK)
    {
        throw new std::runtime_error("failed to remove all hooks");
    }

    if (MH_Uninitialize() != MH_OK)
    {
        throw new std::runtime_error("failed to uninitialize minhook");
    }
}

auto __stdcall Hooks::end_scene(IDirect3DDevice9* device) -> long
{
    long original = end_scene_original(device);

    return original;
}

auto __stdcall Hooks::reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters) -> long
{
    long original = reset_original(device, presentation_parameters);

    return original;
}

constexpr auto Hooks::virtual_function(void* thisptr, size_t index) -> void*
{
    return (*static_cast<void***>(thisptr))[index];
}