#pragma once

#include "../singleton/singleton.hpp"

enum class ConsoleColor
{
	RED,
	GREEN,
	BLUE
};

class Console : public Singleton<Console>
{
public:
    Console();
    ~Console();

    auto attach() -> void;
    auto detach() -> void;
    auto print(const char* format, ...) -> void;
    auto print_color(const char* format, ConsoleColor color, ...) -> void;
};
