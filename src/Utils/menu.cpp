#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>
#include <Windows.h>
#include "config.h"
#include "hacks.h"

bool isMenuClickable = true;
static bool isBindingWallhack = false;

const char* GetKeyName(int vk) {
    if (vk == 0) return "None";
    
    static char keyName[32];
    switch (vk) {
        case VK_LBUTTON: return "LMouse";
        case VK_RBUTTON: return "RMouse";
        case VK_MBUTTON: return "MMouse";
        case VK_XBUTTON1: return "Mouse4";
        case VK_XBUTTON2: return "Mouse5";
        case VK_BACK: return "Backspace";
        case VK_TAB: return "Tab";
        case VK_RETURN: return "Enter";
        case VK_SHIFT: return "Shift";
        case VK_CONTROL: return "Ctrl";
        case VK_MENU: return "Alt";
        case VK_CAPITAL: return "CapsLock";
        case VK_ESCAPE: return "Esc";
        case VK_SPACE: return "Space";
        case VK_INSERT: return "Insert";
        case VK_DELETE: return "Delete";
        case VK_HOME: return "Home";
        case VK_END: return "End";
        case VK_PRIOR: return "PgUp";
        case VK_NEXT: return "PgDn";
        case VK_LEFT: return "Left";
        case VK_UP: return "Up";
        case VK_RIGHT: return "Right";
        case VK_DOWN: return "Down";
        default:
            // F1-F12
            if (vk >= VK_F1 && vk <= VK_F12) {
                sprintf_s(keyName, "F%d", vk - VK_F1 + 1);
                return keyName;
            }
            // 0-9
            if (vk >= 0x30 && vk <= 0x39) {
                sprintf_s(keyName, "%c", vk);
                return keyName;
            }
            // A-Z
            if (vk >= 0x41 && vk <= 0x5A) {
                sprintf_s(keyName, "%c", vk);
                return keyName;
            }
            return "Unknown";
    }
}

void HandleKeyboardInput(HWND window) {
    // F2 для переключения режима меню
    if (GetAsyncKeyState(VK_F2) & 1) {
        isMenuClickable = !isMenuClickable;
        if (isMenuClickable == false) {
            SetWindowLongPtr(window, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED);
        } else {
            SetWindowLongPtr(window, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED);
        }
    }

    // Если ждём назначения клавиши для wallhack
    if (isBindingWallhack) {
        // Проверяем все возможные клавиши
        for (int vk = 0x08; vk <= 0xFE; vk++) {
            // Пропускаем F2 (зарезервирован для меню)
            if (vk == VK_F2) continue;
            
            if (GetAsyncKeyState(vk) & 1) {
                config.wallhackKey = vk;
                isBindingWallhack = false;
                break;
            }
        }
        
        // ESC для отмены
        if (GetAsyncKeyState(VK_ESCAPE) & 1) {
            isBindingWallhack = false;
        }
        
        return; // Не обрабатываем другие клавиши во время привязки
    }

    // Проверка бинда для wallhack
    if (config.wallhackKey != 0) {
        if (GetAsyncKeyState(config.wallhackKey) & 1) {
            config.wallhack = !config.wallhack;
            WallHack();
        }
    }
}

void RenderMenu() {
    ImGui::Begin("t.me/wxsdev | NVIDIA", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    ImGui::Text("Press F2 to toggle menu clickable");
    ImGui::Separator();

    if (ImGui::Checkbox("Wallhack", &config.wallhack)) {
        WallHack();
    }

    ImGui::SameLine();
    
    if (isBindingWallhack) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        if (ImGui::Button("Press any key...")) {
        }
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "ESC to cancel");
    } else {
        char buttonText[64];
        sprintf_s(buttonText, "Bind: %s", GetKeyName(config.wallhackKey));
        
        if (ImGui::Button(buttonText)) {
            isBindingWallhack = true;
        }
        
        if (config.wallhackKey != 0) {
            ImGui::SameLine();
            if (ImGui::Button("Clear")) {
                config.wallhackKey = 0;
            }
        }
    }
    
    ImGui::End();
}