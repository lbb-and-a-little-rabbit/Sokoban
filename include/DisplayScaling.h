#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

// Keep a stable design coordinate system; mapPixelToCoords uses the same viewport.
namespace DisplayScaling {
inline sf::FloatRect viewportFor(sf::Vector2u pixels) {
    if(pixels.x == 0 || pixels.y == 0) return {{0.f,0.f},{1.f,1.f}};
    const float ratio = (static_cast<float>(pixels.x)/pixels.y)/(1920.f/1200.f);
    if(ratio > 1.f) return {{(1.f-1.f/ratio)/2.f,0.f},{1.f/ratio,1.f}};
    return {{0.f,(1.f-ratio)/2.f},{1.f,ratio}};
}
inline void updateView(sf::RenderWindow& window) {
    const auto size = window.getSize();
    if(size.x == 0 || size.y == 0) return;
    sf::View view(sf::FloatRect({0.f,0.f},{1920.f,1200.f}));
    view.setViewport(viewportFor(size));
    window.setView(view);
}
inline bool handleEvent(sf::RenderWindow& window, const sf::Event& event) {
    if(!event.is<sf::Event::Resized>()) return false;
    updateView(window);
    return true;
}
inline void enableDpiAwareness() {
#ifdef _WIN32
    const auto user32 = GetModuleHandleW(L"user32.dll");
    using SetProcessContext = BOOL(WINAPI*)(HANDLE);
    using SetThreadContext = HANDLE(WINAPI*)(HANDLE);
    const auto setProcess = reinterpret_cast<SetProcessContext>(GetProcAddress(user32,"SetProcessDpiAwarenessContext"));
    const auto setThread = reinterpret_cast<SetThreadContext>(GetProcAddress(user32,"SetThreadDpiAwarenessContext"));
    // Per Monitor V2. The thread override also covers SFML's early shared context.
    const auto perMonitorV2 = reinterpret_cast<HANDLE>(static_cast<INT_PTR>(-4));
    if(setProcess) setProcess(perMonitorV2);
    if(setThread) setThread(perMonitorV2);
#endif
}

class WindowAdapter {
#ifdef _WIN32
    HWND handle = nullptr;
    WNDPROC previous = nullptr;
    static constexpr const wchar_t* property = L"Sokoban.DisplayScaling";
    static LRESULT CALLBACK onMessage(HWND hwnd, UINT message, WPARAM wp, LPARAM lp) {
        auto* self = static_cast<WindowAdapter*>(GetPropW(hwnd,property));
        if(!self) return DefWindowProcW(hwnd,message,wp,lp);
        if(message == WM_DPICHANGED) {
            const auto& rect = *reinterpret_cast<const RECT*>(lp);
            SetWindowPos(hwnd,nullptr,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,
                         SWP_NOZORDER|SWP_NOACTIVATE);
            return 0;
        }
        const auto previous = self->previous;
        if(message == WM_NCDESTROY) RemovePropW(hwnd,property);
        return CallWindowProcW(previous,hwnd,message,wp,lp);
    }
#endif
public:
    explicit WindowAdapter(sf::RenderWindow& window) {
#ifdef _WIN32
        handle = window.getNativeHandle();
        if(SetPropW(handle,property,this)) {
            previous = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(handle,GWLP_WNDPROC,
                reinterpret_cast<LONG_PTR>(&onMessage)));
            if(!previous) RemovePropW(handle,property);
        }
        using GetWindowDpi = UINT(WINAPI*)(HWND);
        const auto getDpi = reinterpret_cast<GetWindowDpi>(GetProcAddress(GetModuleHandleW(L"user32.dll"),"GetDpiForWindow"));
        const float dpiScale = getDpi ? std::max(96u,getDpi(handle))/96.f : 1.f;
        MONITORINFO monitor{};
        monitor.cbSize = sizeof(monitor);
        if(GetMonitorInfoW(MonitorFromWindow(handle,MONITOR_DEFAULTTONEAREST),&monitor)) {
            const auto& work = monitor.rcWork;
            const float width = static_cast<float>(work.right-work.left);
            const float height = static_cast<float>(work.bottom-work.top);
            const float scale = std::min({dpiScale,width*.90f/1280.f,height*.85f/800.f});
            window.setSize({static_cast<unsigned>(1280.f*scale),static_cast<unsigned>(800.f*scale)});
            RECT outer{};
            GetWindowRect(handle,&outer);
            window.setPosition({work.left+(work.right-work.left-(outer.right-outer.left))/2,
                                work.top+(work.bottom-work.top-(outer.bottom-outer.top))/2});
        }
#endif
        updateView(window);
    }
    ~WindowAdapter() {
#ifdef _WIN32
        if(previous && IsWindow(handle) && GetPropW(handle,property) == this) {
            SetWindowLongPtrW(handle,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(previous));
            RemovePropW(handle,property);
        }
#endif
    }
    WindowAdapter(const WindowAdapter&) = delete;
    WindowAdapter& operator=(const WindowAdapter&) = delete;
};
}
