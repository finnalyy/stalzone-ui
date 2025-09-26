#include <string>
#include <thread>
#include <array>
#include <iostream>
#include <windows.h>

#include "Drawing.h"
#include "Config.h"
#include "GameData.h"
#include "Utils/Memory.h"

LPCWSTR Drawing::lpWindowName = L"s";
ImVec2 Drawing::vWindowSize = { 500, 530 };
ImGuiWindowFlags Drawing::WindowFlags = 0;
bool Drawing::bDraw = true;

KeyBindState wallhackState, chamsState;

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

bool ImGuiKeyBind(const char* label, int& bind, KeyBindState& state) {

    ImGui::TextUnformatted(label);
    ImGui::SameLine();

    std::string btn_label = state.listening ? "Press" : VkToName(bind);

    if (ImGui::Button(btn_label.c_str())) {
        state.listening = !state.listening;
        if (state.listening) state.temp_bind = bind;
    }

    bool changed = false;

    if (state.listening) {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) { bind = VK_LBUTTON; state.listening = false; changed = true; }
        else if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) { bind = VK_RBUTTON; state.listening = false; changed = true; }
        else if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) { bind = VK_MBUTTON; state.listening = false; changed = true; }

        for (int vk = 0; vk <= 255 && !changed; ++vk) {
            if (GetAsyncKeyState(vk) & 0x8000) {
                if (vk == VK_ESCAPE) { bind = state.temp_bind; state.listening = false; changed = false; }
                else if (vk == VK_DELETE) { bind = 0; state.listening = false; changed = true; }
                else { bind = vk; state.listening = false; changed = true; }
            }
        }
    }
    return changed;
}

bool IsKeyPressedOnce(int vk) {
    if (vk <= 0 || vk > 255) return false;
    static std::array<bool, 256> prev{};
    bool down = (GetAsyncKeyState(vk) & 0x8000) != 0;
    bool pressedOnce = down && !prev[vk];
    prev[vk] = down;
    return pressedOnce;
}

Memory memory;
DWORD processId = 0;

void Drawing::Active() { bDraw = true; }
bool Drawing::isActive() { return bDraw; }

void Drawing::init() {
    if (!Game::isRunning(oxorany("stalcraft.exe"))) {
        MessageBoxW(NULL, oxorany(L"Game is not running!"), oxorany(L"ERROR"), MB_ICONERROR);
        ExitProcess(EXIT_FAILURE);
    }

    processId = Game::getProcessId(oxorany("stalcraft.exe"));
    memory.processId = processId;
    memory.processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!memory.processHandle) {
        MessageBoxW(NULL, oxorany(L"Failed to open process!"), oxorany(L"ERROR"), MB_ICONERROR);
        ExitProcess(EXIT_FAILURE);
    }

    do {
        Globals::baseAddress = memory.GetModuleAddress(oxorany("nvoglv64.dll"));
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    } while (!Globals::baseAddress);

    Wallhack::Init();
    Chams::Init();
}

void Drawing::Draw() {
    if (!isActive()) return;

    float windowWidth = ImGui::GetWindowWidth();
    float windowHeight = ImGui::GetWindowHeight();

    float buttonWidth = 70;
    float buttonHeight = 40;
    float spacing = 10;
    float totalWidth = buttonWidth * 2 + spacing;
    float posX = (windowWidth - totalWidth) / 2 + 100;
    float posY = windowHeight - buttonHeight - 20;
    ImVec2 checkboxPos = ImVec2(ImGui::GetWindowWidth() / 0.8, 20);

    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiStyle originalStyle = style;

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.047f, 0.078f, 0.125f, 1.00f); // #0c1420
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.075f, 0.110f, 0.157f, 1.00f);  // #131c28
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.075f, 0.110f, 0.157f, 1.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.18f, 0.25f, 0.35f, 1.00f);      // #2d404a
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.090f, 0.133f, 0.184f, 1.00f);  // #17222f
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.17f, 0.23f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.08f, 0.12f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.047f, 0.078f, 0.125f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.047f, 0.078f, 0.125f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.047f, 0.078f, 0.125f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.047f, 0.078f, 0.125f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.047f, 0.078f, 0.125f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.18f, 0.25f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.22f, 0.30f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.26f, 0.35f, 0.45f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.20f, 0.80f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.20f, 0.80f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.25f, 0.85f, 0.45f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.22f, 0.32f, 1.00f); 
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.18f, 0.26f, 0.36f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.12f, 0.19f, 0.28f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.80f, 0.40f, 0.30f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.80f, 0.40f, 0.50f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.80f, 0.40f, 0.70f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.18f, 0.25f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.22f, 0.30f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.35f, 0.45f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.80f, 0.40f, 0.20f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.20f, 0.80f, 0.40f, 0.50f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.20f, 0.80f, 0.40f, 0.70f);
    style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

    style.WindowRounding = 12.0f;
    style.ChildRounding = 12.0f;
    style.FrameRounding = 8.0f;
    style.GrabRounding = 8.0f;
    style.ScrollbarRounding = 8.0f;
    style.TabRounding = 8.0f;
    style.WindowPadding = ImVec2(20, 20);
    style.FramePadding = ImVec2(10, 6);
    style.ItemSpacing = ImVec2(12, 12);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 10.0f;
    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    
    ImVec2 windowSize(600, 500);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("##", &bDraw, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
    {
        switch (menuState.selectedTab) {
            case 0:
                ImGui::BeginChild(oxorany("Wallhack"), ImVec2(0, 80), false); 
                ImGui::SetCursorPos(ImVec2(20, 25));
                ImGui::Text(oxorany("Wallhack"));
                ImGui::SameLine();
                ImGui::SetCursorPos(checkboxPos);
                ImGui::Checkbox(oxorany("##wallhack"), &Globals::g_wallhack_C);
                if (Globals::g_wallhack_C)
                {
                ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 1.5, 20));
                ImGuiKeyBind("", Globals::g_wallhackBind, wallhackState);
                if (Globals::g_wallhackBind != 0 && IsKeyPressedOnce(Globals::g_wallhackBind))
                {
                    Wallhack::toggle(Globals::g_wallhack, memory.processHandle);
                }
                }
                ImGui::EndChild();
                // ------- CHAMS -------    
                ImGui::BeginChild(oxorany("Chams"), ImVec2(0, 80), false); 
                ImGui::SetCursorPos(ImVec2(20, 25));
                ImGui::Text(oxorany("Chams (visible only)"));
                ImGui::SameLine();
                ImGui::SetCursorPos(checkboxPos);
                ImGui::Checkbox(oxorany("##Chams"), &Globals::g_chams_C);
                if (Globals::g_chams_C)
                {
                ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 1.5, 20));
                ImGuiKeyBind("", Globals::g_chamsBind, chamsState);
                if (Globals::g_chamsBind != 0 && IsKeyPressedOnce(Globals::g_chamsBind))
                {
                    Chams::toggle(Globals::g_chams, memory.processHandle);
                }
                }
                ImGui::EndChild();
            break;
            case 1:
                ImGui::BeginChild(oxorany("##Settings"), ImVec2(0, 80), false);
                ImGui::SetCursorPos(ImVec2(20, 25));
                ImGui::Text(oxorany("Telegram: t.me/pmquestor"));
                ImGui::EndChild();
            break;
        }

        ImGui::SetCursorPos(ImVec2(posX, posY + 100));
        if (ImGui::Button(oxorany("P"), ImVec2(buttonWidth, buttonHeight))) menuState.selectedTab = 0;

        ImGui::SameLine(posX + buttonWidth + spacing);
        if (ImGui::Button(oxorany("M"), ImVec2(buttonWidth, buttonHeight))) menuState.selectedTab = 1;

        ImGui::SameLine(posX + buttonWidth + spacing - 300);
        if (ImGui::Button(oxorany("X"), ImVec2(buttonWidth, buttonHeight))) bDraw = false;
    }
    ImGui::End();

    // style = originalStyle;

}
