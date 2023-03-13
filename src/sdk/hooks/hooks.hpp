#pragma once

#include "../../common/singleton/singleton.hpp"

#include <d3d9.h>

class Hooks : public Singleton<Hooks>
{
public:
	Hooks();
	~Hooks();

	auto setup() -> void;
	auto destroy() -> void;

private:
	auto __stdcall end_scene(IDirect3DDevice9* device) -> long;
	auto __stdcall reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters) -> long;

	typedef long(__stdcall* EndScene)(IDirect3DDevice9*);
	typedef long(__stdcall* Reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

	EndScene end_scene_original = nullptr;
	Reset reset_original = nullptr;

	constexpr auto virtual_function(void* thisptr, size_t index) -> void*;
};

