#include "console.hpp"

#include <iostream>
#include <stdexcept>
#include <Windows.h>

// These functions provide console functionality for debugging. To avoid visual bloat in release
// builds, we use `#ifndef _DEBUG` to exclude the code in the header file from compilation when
// `_DEBUG` is defined, rather than at the function call site.
//  - veasman

Console::Console() { }

Console::~Console() { }

auto Console::attach() -> void
{
    if (!AllocConsole())
    {
        throw std::runtime_error("failed to allocate console");
    }

    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    SetConsoleTitleA("debug console");
}

auto Console::detach() -> void
{
    FreeConsole();
}

auto Console::print(const char* format, ...) -> void
{
    const char* project_name = PROJECT_NAME;

    if (project_name == nullptr)
    {
        project_name = "cheat-sdk";
    }

    std::string prepended_format = std::string("[") + project_name + std::string("] ") + format;

    va_list arguments;
    va_start(arguments, format);
    vprintf_s(prepended_format.c_str(), arguments);
    va_end(arguments);
}

auto Console::print_color(const char* format, ConsoleColor color, ...) -> void
{
    const char* project_name = PROJECT_NAME;

    if (project_name == nullptr)
    {
        project_name = "cheat-sdk";
    }

    va_list arguments;
    va_start(arguments, color);

    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    switch (color)
    {
    case ConsoleColor::RED:
        SetConsoleTextAttribute(console_handle, FOREGROUND_RED);
        break;
    case ConsoleColor::GREEN:
        SetConsoleTextAttribute(console_handle, FOREGROUND_GREEN);
        break;
    case ConsoleColor::BLUE:
        SetConsoleTextAttribute(console_handle, FOREGROUND_BLUE);
        break;
    default:
        SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    }

    std::string prepended_format = std::string("[") + project_name + std::string("] ") + format;

    vprintf_s(format, arguments);

    SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    va_end(arguments);
} 
