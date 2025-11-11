#pragma once

#include "pch.h"

class Drawing
{
private:
    static LPCWSTR lpWindowName;;
    static ImVec2 vWindowSize;
    static ImGuiWindowFlags WindowFlags;
    static bool bDraw;

public:
    static void Active();
    static bool isActive();
    static void Draw();
    static std::string VkToName(int vk) {
        if (vk == 0) return "None";
        if ((vk >= 'A' && vk <= 'Z') || (vk >= '0' && vk <= '9')) {
            char buf[2] = { (char)vk, 0 };
            return std::string(buf);
        }
        switch (vk) {
            case VK_SPACE: return "Space"; case VK_ESCAPE: return "Esc"; case VK_RETURN: return "Enter";
            case VK_BACK: return "Backspace"; case VK_TAB: return "Tab"; case VK_SHIFT: return "Shift";
            case VK_CONTROL: return "Ctrl"; case VK_MENU: return "Alt"; case VK_LWIN: return "LWin";
            case VK_RWIN: return "RWin"; case VK_LEFT: return "Left"; case VK_RIGHT: return "Right";
            case VK_UP: return "Up"; case VK_DOWN: return "Down"; case VK_INSERT: return "Insert";
            case VK_DELETE: return "Delete"; case VK_PRIOR: return "PageUp"; case VK_NEXT: return "PageDown";
            case VK_HOME: return "Home"; case VK_END: return "End"; case VK_F1: return "F1"; case VK_F2: return "F2";
            case VK_F3: return "F3"; case VK_F4: return "F4"; case VK_F5: return "F5"; case VK_F6: return "F6";
            case VK_F7: return "F7"; case VK_F8: return "F8"; case VK_F9: return "F9"; case VK_F10: return "F10";
            case VK_F11: return "F11"; case VK_F12: return "F12";
            default: return "VK_" + std::to_string(vk);
        }
    }
    static void init();
};