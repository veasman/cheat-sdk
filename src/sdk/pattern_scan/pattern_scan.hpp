#pragma once

#include "../../common/singleton/singleton.hpp"

#include <cstdint>

class PatternScan : public Singleton<PatternScan>
{
public:
	auto scan(void* module, const char* signature) -> std::uint8_t*;
};

