#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <imm.h>
#endif

// This game has no text entry. Detach IME only from its own window, keeping
// the user's keyboard layout and IME settings for other applications intact.
class GameWindowInput {
#ifdef _WIN32
    HMODULE library = nullptr;
    HWND window = nullptr;
    HIMC previous = nullptr;
    using AssociateContext = HIMC(WINAPI*)(HWND, HIMC);
    AssociateContext associate = nullptr;
#endif
public:
    explicit GameWindowInput(sf::RenderWindow& target) {
#ifdef _WIN32
        window = target.getNativeHandle();
        library = LoadLibraryW(L"imm32.dll");
        if(library) {
            associate = reinterpret_cast<AssociateContext>(GetProcAddress(library,"ImmAssociateContext"));
            if(associate) previous = associate(window,nullptr);
        }
#endif
    }
    ~GameWindowInput() {
#ifdef _WIN32
        if(associate && IsWindow(window)) associate(window,previous);
        if(library) FreeLibrary(library);
#endif
    }
    GameWindowInput(const GameWindowInput&) = delete;
    GameWindowInput& operator=(const GameWindowInput&) = delete;
};
