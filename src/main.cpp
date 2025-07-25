#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Windows.h>

using namespace geode::prelude;

WNDPROC originalWndProc = nullptr;

LRESULT CALLBACK myWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_ACTIVATE) {
        auto state = LOWORD(wParam);
        if (state == WA_INACTIVE) {
            if (PlayLayer::get() && !PlayLayer::get()->m_isPaused) {
                log::info("Window lost focus -> pausing game");
                PlayLayer::get()->pauseGame(true);
            }
        }
    }
    return CallWindowProcW(originalWndProc, hwnd, msg, wParam, lParam);
}

$on_mod(Loaded) {
    HWND hwnd = GetActiveWindow();
    if (hwnd) {
        originalWndProc = (WNDPROC)SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR)myWndProc);
        log::info("WndProc hook set");
    } else {
        log::warn("Failed to get HWND");
    }
}