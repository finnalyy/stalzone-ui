// #include <string>
// #include <thread>
// #include <array>
// #include <iostream>
// #include <windows.h>
// #include <map>

// #include "Drawing.h"
// #include "offsets.h"
// #include "Config.h"
// // #include "memory_func.h"

// #include <imgui.h>
// #include <imgui_internal.h>

// LPCWSTR Drawing::lpWindowName = L"s";
// ImVec2  Drawing::vWindowSize = { 500, 530 };
// ImGuiWindowFlags Drawing::WindowFlags = 0;
// bool Drawing::bDraw = true;
// const char* NameProcess = "";
// static int currentTab = 0;
// KeyBindState wallhackState, chamsState;

// namespace Colors {
//     ImVec4 Background = ImVec4(0.07f, 0.07f, 0.09f, 0.98f);
//     ImVec4 TitleBar = ImVec4(0.10f, 0.10f, 0.12f, 1.0f);
//     ImVec4 Accent = ImVec4(0.65f, 0.45f, 0.95f, 1.0f);
//     ImVec4 AccentDim = ImVec4(0.45f, 0.35f, 0.65f, 1.0f);
//     ImVec4 AccentGlow = ImVec4(0.65f, 0.45f, 0.95f, 0.3f);
//     ImVec4 Text = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
//     ImVec4 TextDim = ImVec4(0.55f, 0.55f, 0.6f, 1.0f);
//     ImVec4 CardBg = ImVec4(0.12f, 0.12f, 0.15f, 1.0f);
//     ImVec4 CardHover = ImVec4(0.15f, 0.15f, 0.19f, 1.0f);
//     ImVec4 Border = ImVec4(0.18f, 0.18f, 0.22f, 1.0f);
//     ImVec4 ToggleActive = ImVec4(0.65f, 0.45f, 0.95f, 1.0f);
//     ImVec4 ToggleInactive = ImVec4(0.25f, 0.25f, 0.3f, 1.0f);
// }

// bool ImGuiKeyBind(const char* label, int& bind, KeyBindState& state) {
//     ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.75f, 1.0f));
//     ImGui::TextUnformatted(label);
//     ImGui::PopStyleColor();
//     ImGui::SameLine();

//     std::string btn_label = state.listening ? "Press" : Drawing::VkToName(bind);

//     ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.18f, 0.18f, 0.22f, 1.0f));
//     ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.25f, 0.3f, 1.0f));
//     ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.4f, 0.9f, 0.4f));
//     ImGui::PushStyleColor(ImGuiCol_Text, state.listening ? ImVec4(0.6f, 0.4f, 0.9f, 1.0f) : ImVec4(0.9f, 0.9f, 0.9f, 1.0f));

//     bool pressed = ImGui::Button(btn_label.c_str(), ImVec2(120, 0));
    
//     ImGui::PopStyleColor(4);

//     if (pressed) {
//         state.listening = !state.listening;
//         if (state.listening) {
//             state.temp_bind = bind;
//             state.keys_released = false; // Сбрасываем флаг для этого конкретного состояния
//         }
//     }

//     bool changed = false;

//     if (state.listening) {
//         // Ждём пока все клавиши будут отпущены перед началом захвата
//         if (!state.keys_released) {
//             bool anyKeyDown = false;
//             for (int vk = 1; vk <= 255; ++vk) {
//                 if (GetAsyncKeyState(vk) & 0x8000) {
//                     anyKeyDown = true;
//                     break;
//                 }
//             }
//             if (!anyKeyDown) {
//                 state.keys_released = true;
//             }
//             return false;
//         }

//         // Теперь можем захватывать клавиши
//         if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { 
//             bind = state.temp_bind; 
//             state.listening = false;
//             state.keys_released = false;
//         } else if (GetAsyncKeyState(VK_DELETE) & 0x8000) { 
//             bind = 0; 
//             state.listening = false; 
//             changed = true;
//             state.keys_released = false;
//         } else {
//             for (int vk = 8; vk <= 255; ++vk) {
//                 if (vk == VK_LBUTTON || vk == VK_RBUTTON || vk == VK_MBUTTON) continue;
                
//                 if (GetAsyncKeyState(vk) & 0x8000) {
//                     bind = vk;
//                     state.listening = false;
//                     changed = true;
//                     state.keys_released = false;
//                     break;
//                 }
//             }
//         }
//     }
//     return changed;
// }

// bool IsKeyPressedOnce(int vk) {
//     if (vk <= 0 || vk > 255) return false;
//     static std::array<bool, 256> prev{};
//     bool down = (GetAsyncKeyState(vk) & 0x8000) != 0;
//     bool pressedOnce = down && !prev[vk];
//     prev[vk] = down;
//     return pressedOnce;
// }

// void DrawToggle(const char* label, bool* value, float width = 46.0f) {
//     ImGuiWindow* window = ImGui::GetCurrentWindow();
//     if (window->SkipItems) return;

//     ImGuiContext& g = *GImGui;
//     const ImGuiStyle& style = g.Style;
//     const ImGuiID id = window->GetID(label);

//     const float height = 24.0f;
//     const ImVec2 pos = window->DC.CursorPos;
//     const ImRect total_bb(pos, ImVec2(pos.x + width, pos.y + height));

//     ImGui::ItemSize(total_bb, style.FramePadding.y);
//     if (!ImGui::ItemAdd(total_bb, id)) return;

//     bool hovered, held;
//     bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
//     if (pressed) *value = !*value;

//     // Анимация
//     static std::map<ImGuiID, float> animations;
//     float& anim = animations[id];
//     float target = *value ? 1.0f : 0.0f;
//     anim = ImLerp(anim, target, g.IO.DeltaTime * 12.0f);

//     // Фон
//     ImU32 col_bg = ImGui::GetColorU32(ImLerp(Colors::ToggleInactive, Colors::ToggleActive, anim));
//     window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, col_bg, height * 0.5f);

//     // Свечение при hover
//     if (hovered) {
//         window->DrawList->AddRect(total_bb.Min, total_bb.Max, 
//             ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.15f)), height * 0.5f, 0, 1.5f);
//     }

//     // Индикатор
//     float circle_radius = height * 0.35f;
//     float circle_pos_x = ImLerp(total_bb.Min.x + circle_radius + 3.0f, 
//                                  total_bb.Max.x - circle_radius - 3.0f, anim);
//     ImVec2 circle_center(circle_pos_x, total_bb.Min.y + height * 0.5f);
    
//     // Тень индикатора
//     window->DrawList->AddCircleFilled(
//         ImVec2(circle_center.x, circle_center.y + 1), circle_radius, 
//         IM_COL32(0, 0, 0, 50), 16);
    
//     window->DrawList->AddCircleFilled(circle_center, circle_radius, 
//         IM_COL32(255, 255, 255, 255), 16);
// }

// void DrawFeatureCard(const char* title, bool* enabled, 
//                      int* keybind, KeyBindState& state, const ImVec2& size) {
//     ImDrawList* draw_list = ImGui::GetWindowDrawList();
//     ImVec2 pos = ImGui::GetCursorScreenPos();
//     ImVec2 endPos(pos.x + size.x, pos.y + size.y);
    
//     draw_list->AddRectFilled(pos, endPos, ImGui::GetColorU32(Colors::CardBg), 8.0f);
    
//     if (ImGui::IsMouseHoveringRect(pos, endPos)) {
//         draw_list->AddRect(pos, endPos, ImGui::GetColorU32(Colors::Accent), 8.0f, 0, 1.5f);
//     }
    
//     ImGui::BeginGroup();
//     ImGui::Dummy(ImVec2(size.x, size.y));
//     ImGui::EndGroup();
    
//     ImGui::SetCursorScreenPos(ImVec2(pos.x + 16, pos.y + 25));
//     ImGui::BeginGroup();
    
//     ImGui::PushStyleColor(ImGuiCol_Text, Colors::Accent);
//     ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
//     ImGui::Text("%s", title);
//     ImGui::PopFont();
//     ImGui::PopStyleColor();
    
//     ImGui::EndGroup();
    
//     ImGui::SetCursorScreenPos(ImVec2(endPos.x - 62, pos.y + 25));
//     std::string toggleId = "##toggle_" + std::string(title);
//     DrawToggle(toggleId.c_str(), enabled);
    
//     if (*enabled) {
//         ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + size.y - 50));
//         ImGui::PushItemWidth(size.x - 32);
//         ImGuiKeyBind("", *keybind, state);
//         ImGui::PopItemWidth();
//     }
    
//     ImGui::SetCursorScreenPos(ImVec2(pos.x, endPos.y + 12));
// }


// void Drawing::Active() { bDraw = true; }
// bool Drawing::isActive() { return bDraw; }

// void Drawing::init() {
//     // int GameClient = MessageBoxW(NULL, oxorany(L"Yes - Steam\nNo - EXBO Launcher"), oxorany(L"Select game client"), MB_YESNO | MB_ICONQUESTION);
//     // if (GameClient == IDYES) {
//     //     NameProcess = "stalcraftw.exe";
//     // } else if (GameClient == IDNO) {
//     //     NameProcess = "stalcraft.exe";
//     // }

//     // if (!Game::isRunning(oxorany(NameProcess))) {
//     //     MessageBoxW(NULL, oxorany(L"Game is not running!"), oxorany(L"Error"), MB_ICONERROR);
//     //     ExitProcess(EXIT_FAILURE);
//     // }

//     // processId = Game::getProcessId(oxorany("stalcraftw.exe"));
//     // memory.processId = processId;
//     // memory.processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

//     // if (!memory.processHandle) {
//     //     MessageBoxW(NULL, oxorany(L"Failed to open process!"), oxorany(L"Error"), MB_ICONERROR);
//     //     ExitProcess(EXIT_FAILURE);
//     // }

//     // do {
//     //     Globals::baseAddress = memory.GetModuleAddress(oxorany("nvoglv64.dll"));
//     //     std::this_thread::sleep_for(std::chrono::milliseconds(200));
//     // } while (!Globals::baseAddress);

// }

// void Drawing::Draw()
// {
//     if (!bDraw)
//         return;

//     // ---------------- STYLE (лучше вынести в init, но оставляем тут)
//     ImGuiStyle& style = ImGui::GetStyle();
//     style.WindowRounding = 12.0f;
//     style.FrameRounding = 6.0f;
//     style.ScrollbarRounding = 6.0f;
//     style.GrabRounding = 4.0f;
//     style.WindowPadding = ImVec2(0, 0);
//     // style.FramePadding = ImVec2(10, 7);
//     style.ItemSpacing = ImVec2(14, 14);
//     style.ScrollbarSize = 10.0f;
//     ImGui::SetNextWindowSize(ImVec2(520, 480), ImGuiCond_FirstUseEver);

//     ImGui::PushStyleColor(ImGuiCol_WindowBg, Colors::Background);
//     ImGui::PushStyleColor(ImGuiCol_Border, Colors::Border);
//     ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

//     if (!ImGui::Begin(
//         "##main",
//         nullptr,
//         ImGuiWindowFlags_NoResize |
//         ImGuiWindowFlags_NoCollapse |
//         ImGuiWindowFlags_NoTitleBar |
//         ImGuiWindowFlags_NoScrollbar))
//     {
//         ImGui::PopStyleVar();
//         ImGui::PopStyleColor(2);
//         ImGui::End();
//         return;
//     }

//     // ================= WINDOW DATA =================
//     ImDrawList* draw = ImGui::GetWindowDrawList();
//     ImVec2 winPos = ImGui::GetWindowPos();
//     ImVec2 winSize = ImGui::GetWindowSize();
//     ImGuiIO& io = ImGui::GetIO();

//     constexpr float headerHeight = 65.0f;

//     // ================= HEADER =================
//     draw->AddRectFilled(
//         winPos,
//         ImVec2(winPos.x + winSize.x, winPos.y + headerHeight),
//         ImGui::GetColorU32(Colors::TitleBar),
//         12.0f,
//         ImDrawFlags_RoundCornersTop
//     );

//     // Logo
//     ImGui::SetCursorPos(ImVec2(24, 20));
//     ImGui::PushStyleColor(ImGuiCol_Text, Colors::Accent);
//     ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
//     ImGui::Text("WXS");
//     ImGui::PopFont();
//     ImGui::PopStyleColor();

//     ImGui::SetCursorPos(ImVec2(winSize.x - 44, 20));
//     ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
//     ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.2f, 0.2f, 0.8f));
//     ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
//     ImGui::PushStyleColor(ImGuiCol_Text, Colors::Text);
//     if (ImGui::Button("##exit", ImVec2(28, 28))) {
//         PostQuitMessage(0);
//     }
//     ImGui::PopStyleColor(4);

//     // ================= TABS =================
//     ImGui::SetCursorPos(ImVec2((winSize.x - 160) * 0.5f, 20));

//     ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
//     ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colors::AccentGlow);
//     ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.4f, 0.9f, 0.2f));

//     bool clickHome = false;
//     bool clickMisc = false;

//     float homeX = 0.0f;
//     float miscX = 0.0f;

//     ImGui::PushStyleColor(ImGuiCol_Text, currentTab == 0 ? Colors::Text : Colors::TextDim);
//     if (ImGui::Button("NVIDIA", ImVec2(110, 40)))
//         clickHome = true;
//     {
//         ImVec2 min = ImGui::GetItemRectMin();
//         ImVec2 max = ImGui::GetItemRectMax();
//         homeX = (min.x + max.x) * 0.5f;
//     }
//     ImGui::PopStyleColor();

//     ImGui::SameLine();

//     ImGui::PushStyleColor(ImGuiCol_Text, currentTab == 1 ? Colors::Text : Colors::TextDim);
//     if (ImGui::Button("AMD", ImVec2(70, 40)))
//         clickMisc = true;
//     {
//         ImVec2 min = ImGui::GetItemRectMin();
//         ImVec2 max = ImGui::GetItemRectMax();
//         miscX = (min.x + max.x) * 0.5f;
//     }
//     ImGui::PopStyleColor();

//     ImGui::PopStyleColor(3);

//     // ================= TAB INDICATOR =================
//     static float indicatorX = -1.0f;
//     if (indicatorX < 0.0f)
//         indicatorX = (currentTab == 0) ? homeX : miscX;

//     float targetX = (currentTab == 0) ? homeX : miscX;
//     indicatorX = ImLerp(indicatorX, targetX, io.DeltaTime * 14.0f);

//     float indicatorY = headerHeight + winPos.y + 4.0f;
//     draw->AddCircleFilled(
//         ImVec2(indicatorX, indicatorY),
//         3.0f,
//         ImGui::GetColorU32(Colors::Accent),
//         12
//     );

//     if (clickHome) currentTab = 0;
//     if (clickMisc) currentTab = 1;

//     draw->AddLine(
//         ImVec2(winPos.x + 20, winPos.y + headerHeight),
//         ImVec2(winPos.x + winSize.x - 20, winPos.y + headerHeight),
//         ImGui::GetColorU32(Colors::Border),
//         2.0f
//     );

//     // ================= CONTENT =================
//     ImGui::SetCursorPos(ImVec2(20, headerHeight + 20));
//     if (ImGui::BeginChild(
//         "##Content",
//         ImVec2(winSize.x - 40, winSize.y - headerHeight - 40),
//         false,
//         ImGuiWindowFlags_NoScrollbar))
//     {
//         // if (currentTab == 0)
//         // {
//         //     ImVec2 cardSize(winSize.x - 60, Globals::g_wallhack_C ? 120 : 80);
//         //     DrawFeatureCard("Wallhack",
//         //         &Globals::g_wallhack_C, 
//         //         &Globals::g_wallhackBind, 
//         //         wallhackState, 
//         //         cardSize);

//         //     if (Globals::g_wallhack_C && Globals::g_wallhackBind &&
//         //         IsKeyPressedOnce(Globals::g_wallhackBind))
//         //     {
//         //         Wallhack::toggle(Globals::g_wallhack, memory.processHandle);
//         //     }

//         //     cardSize.y = Globals::g_chams_C ? 120 : 80;
//         //     DrawFeatureCard("Chams", 
//         //         &Globals::g_chams_C, 
//         //         &Globals::g_chamsBind, 
//         //         chamsState, 
//         //         cardSize);

//         //     if (Globals::g_chams_C && Globals::g_chamsBind &&
//         //         IsKeyPressedOnce(Globals::g_chamsBind))
//         //     {
//         //         Chams::toggle(Globals::g_chams, memory.processHandle);
//         //     }
//         // }
//     }
//     ImGui::EndChild();

//     // ================= CLEANUP =================
//     ImGui::End();
//     ImGui::PopStyleVar();
//     ImGui::PopStyleColor(2);
// }
