#include <string>
#include <thread>
#include <array>
#include <iostream>
#include <windows.h>

#include "Drawing.h"
#include "Config.h"
#include "GameData.h"
#include "Utils/Memory.h"
#include <imgui_internal.h>

LPCWSTR Drawing::lpWindowName = L"s";
ImVec2 Drawing::vWindowSize = { 500, 530 };
ImGuiWindowFlags Drawing::WindowFlags = 0;
bool Drawing::bDraw = true;

static int currentTab = 0;

KeyBindState wallhackState, chamsState;

bool ImGuiKeyBind(const char* label, int& bind, KeyBindState& state) {

    ImGui::TextUnformatted(label);
    ImGui::SameLine();

    std::string btn_label = state.listening ? "Press" : Drawing::VkToName(bind);

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
    // if (!Game::isRunning(oxorany("stalcraft.exe"))) {
    //     MessageBoxW(NULL, oxorany(L"Game is not running!"), oxorany(L"Error"), MB_ICONERROR);
    //     ExitProcess(EXIT_FAILURE);
    // }   

    processId = Game::getProcessId(oxorany("stalcraft.exe"));
    memory.processId = processId;
    memory.processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    // if (!memory.processHandle) {
    //     MessageBoxW(NULL, oxorany(L"Failed to open process!"), oxorany(L"Error"), MB_ICONERROR);
    //     ExitProcess(EXIT_FAILURE);
    // }

    // do {
    //     Globals::baseAddress = memory.GetModuleAddress(oxorany("nvoglv64.dll"));
    //     std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // } while (!Globals::baseAddress);

    Chams::Init(Globals::baseAddress, Globals::chamsAddr);
    Wallhack::Init(Globals::baseAddress, Globals::wallhackAddr);

}

namespace Colors {
    ImVec4 Background = ImVec4(0.08f, 0.08f, 0.10f, 0.95f);
    ImVec4 TitleBar = ImVec4(0.12f, 0.12f, 0.14f, 1.0f);
    ImVec4 Purple = ImVec4(0.6f, 0.4f, 0.9f, 1.0f);
    ImVec4 PurpleDim = ImVec4(0.4f, 0.3f, 0.6f, 1.0f);
    ImVec4 Text = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    ImVec4 TextDim = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
    ImVec4 ButtonBg = ImVec4(0.15f, 0.15f, 0.18f, 1.0f);
    ImVec4 ButtonHover = ImVec4(0.2f, 0.2f, 0.24f, 1.0f);
    ImVec4 ToggleActive = ImVec4(0.6f, 0.4f, 0.9f, 1.0f);
    ImVec4 ToggleInactive = ImVec4(0.3f, 0.3f, 0.35f, 1.0f);
}

void DrawToggle(const char* label, bool* value, float width = 40.0f) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    const float height = 20.0f;
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, ImVec2(pos.x + width, pos.y + height));

    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id))
        return;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
        *value = !*value;

    float t = *value ? 1.0f : 0.0f;

    // Рисуем фон переключателя
    ImU32 col_bg = *value ? ImGui::GetColorU32(Colors::ToggleActive) : ImGui::GetColorU32(Colors::ToggleInactive);
    window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, col_bg, height * 0.5f);

    // Рисуем круглый индикатор
    float circle_radius = height * 0.35f;
    float circle_pos_x = *value ? (total_bb.Max.x - circle_radius - 3.0f) : (total_bb.Min.x + circle_radius + 3.0f);
    ImVec2 circle_center(circle_pos_x, total_bb.Min.y + height * 0.5f);
    window->DrawList->AddCircleFilled(circle_center, circle_radius, IM_COL32(255, 255, 255, 255));
}

void Drawing::Draw() {
    if (!isActive()) return;

    ImGuiStyle& style = ImGui::GetStyle();
    
    style.WindowRounding = 8.0f;
    style.FrameRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.WindowPadding = ImVec2(0, 0);
    style.FramePadding = ImVec2(8, 6);
    style.ItemSpacing = ImVec2(12, 12);

    ImGui::SetNextWindowSize(ImVec2(500, 450), ImGuiCond_FirstUseEver);
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, Colors::Background);
    ImGui::PushStyleColor(ImGuiCol_TitleBg, Colors::TitleBar);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, Colors::TitleBar);
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2f, 0.2f, 0.25f, 1.0f));
    
    if (ImGui::Begin("##", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)) {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 window_pos = ImGui::GetWindowPos();
        ImVec2 window_size = ImGui::GetWindowSize();

        ImGui::SetCursorPos(ImVec2(20, 15));
        ImGui::PushStyleColor(ImGuiCol_Text, Colors::Purple);
        ImGui::Text(oxorany("KILLCHEAT"));
        ImGui::PopStyleColor();

        ImGuiIO& io = ImGui::GetIO();
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 windowPos = ImGui::GetWindowPos();

        static float underlineLocalPos = 0.0f;
        static float underlineWidth = 0.0f;
        static float targetLocalPos = 0.0f;
        static float targetWidth = 0.0f;
        const float animSpeed = 12.0f;

        ImGui::SameLine(250);
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.25f, 0.2f, 0.35f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.6f, 0.4f, 0.9f, 0.2f));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.6f, 0.4f, 0.9f, 0.35f));
        ImGui::PushStyleColor(ImGuiCol_Text, currentTab == 0 ? Colors::Text : Colors::TextDim);

        bool clickedHome = ImGui::Selectable(oxorany("Home"), currentTab == 0, 0, ImVec2(60, 25));
        ImVec2 homeMin = ImGui::GetItemRectMin();
        ImVec2 homeMax = ImGui::GetItemRectMax();
        ImVec2 homeCenter = ImVec2((homeMin.x + homeMax.x) * 0.5f, homeMax.y);
        float homeWidth = homeMax.x - homeMin.x;

        float homeCenterLocal = homeCenter.x - windowPos.x;
        float homeYLocal = homeMax.y - windowPos.y;

        ImGui::PopStyleColor(4);
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.25f, 0.2f, 0.35f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.6f, 0.4f, 0.9f, 0.2f));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.6f, 0.4f, 0.9f, 0.35f));
        ImGui::PushStyleColor(ImGuiCol_Text, currentTab == 1 ? Colors::Text : Colors::TextDim);

        bool clickedMisc = ImGui::Selectable(oxorany("Misc"), currentTab == 1, 0, ImVec2(60, 25));
        ImVec2 miscMin = ImGui::GetItemRectMin();
        ImVec2 miscMax = ImGui::GetItemRectMax();
        ImVec2 miscCenter = ImVec2((miscMin.x + miscMax.x) * 0.5f, miscMax.y);
        float miscWidth = miscMax.x - miscMin.x;

        float miscCenterLocal = miscCenter.x - windowPos.x;
        float miscYLocal = miscMax.y - windowPos.y;

        ImGui::PopStyleColor(4);

        if (clickedHome && currentTab != 0) {
            currentTab = 0;
            targetLocalPos = homeCenterLocal;
            targetWidth = homeWidth;
        }
        if (clickedMisc && currentTab != 1) {
            currentTab = 1;
            targetLocalPos = miscCenterLocal;
            targetWidth = miscWidth;
        }

        static bool initialized = false;
        if (!initialized) {
            initialized = true;
            if (currentTab == 0) {
                underlineLocalPos = homeCenterLocal;
                underlineWidth = homeWidth;
                targetLocalPos = homeCenterLocal;
                targetWidth = homeWidth;
            } else {
                underlineLocalPos = miscCenterLocal;
                underlineWidth = miscWidth;
                targetLocalPos = miscCenterLocal;
                targetWidth = miscWidth;
            }
        }

        // --- Анимация ---
        underlineLocalPos = ImLerp(underlineLocalPos, targetLocalPos, io.DeltaTime * animSpeed);
        underlineWidth = ImLerp(underlineWidth, targetWidth, io.DeltaTime * animSpeed);

        // Линия
        float underlineY = windowPos.y + homeYLocal + 0.0f;
        float halfWidth = underlineWidth * 0.5f;
        float underlineX = windowPos.x + underlineLocalPos;
        dl->AddLine(ImVec2(underlineX - halfWidth, underlineY),
                    ImVec2(underlineX + halfWidth, underlineY),
                    ImGui::GetColorU32(Colors::Purple), 2.0f);

        ImGui::SameLine();
        ImGui::SetCursorPosX(window_size.x - 35);
        if (ImGui::Selectable(oxorany("X"), false, 0, ImVec2(20, 20))) {
            exit(0);
        }

        draw_list->AddLine(
            ImVec2(window_pos.x + 20, window_pos.y + 55),
            ImVec2(window_pos.x + window_size.x - 20, window_pos.y + 55),
            IM_COL32(40, 40, 45, 255)
        );

        ImGui::SetCursorPos(ImVec2(20, 80));
        ImGui::BeginChild(oxorany("##Content"), ImVec2(window_size.x - 40, window_size.y - 100), false);

        if (currentTab == 0) { // Home Tab

            ImGui::BeginGroup();
            
            ImGui::PushStyleColor(ImGuiCol_Text, Colors::Purple);
            ImGui::Text(oxorany("Wallhack"));
            ImGui::PopStyleColor();
            ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextDim);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 0);
            ImGui::Text(oxorany("Safe"));
            ImGui::PopStyleColor();

            ImGui::SameLine(380);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 35);
            DrawToggle(oxorany("##wallhack"), &Globals::g_wallhack_C);

            if (Globals::g_wallhack_C)
            {
                ImGuiKeyBind(oxorany(""), Globals::g_wallhackBind, wallhackState);
                if (Globals::g_wallhackBind != 0 && IsKeyPressedOnce(Globals::g_wallhackBind))
                {
                    Wallhack::toggle(Globals::g_wallhack, memory.processHandle);
                }
            }

            ImGui::Dummy(ImVec2(0, 15));

            // Chams 
            ImGui::PushStyleColor(ImGuiCol_Text, Colors::Purple);
            ImGui::Text(oxorany("Chams"));
            ImGui::PopStyleColor();
            ImGui::PushStyleColor(ImGuiCol_Text, Colors::TextDim);
            ImGui::Text(oxorany("Only visible"));
            ImGui::PopStyleColor();

            ImGui::SameLine(380);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 25);
            DrawToggle(oxorany("##Chams"), &Globals::g_chams_C);

            if (Globals::g_chams_C)
            {
                ImGuiKeyBind(oxorany(""), Globals::g_chamsBind, chamsState);
                if (Globals::g_chamsBind != 0 && IsKeyPressedOnce(Globals::g_chamsBind))
                {
                    Chams::toggle(Globals::g_chams, memory.processHandle);
                }
            }
            ImGui::EndGroup();
        } else if (currentTab == 1) {
            // ImGui::SetCursorPosX((window_size.x - 40 - 400) / 2);
            ImGui::PushStyleColor(ImGuiCol_Button, Colors::ButtonBg);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::ButtonHover);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colors::ButtonBg);
            ImGui::PushStyleColor(ImGuiCol_Text, Colors::Text);
            
            if (ImGui::Button(oxorany("Save config"), ImVec2(222, 40))) {
            }
            ImGui::SameLine();
            if (ImGui::Button(oxorany("Load config"), ImVec2(222, 40))) {
            }
            
            ImGui::PopStyleColor(4);

            ImGui::Text("t.me/telegram");
        }

        ImGui::EndChild();
        ImGui::End();
    }
    
    ImGui::PopStyleColor(4);
}
