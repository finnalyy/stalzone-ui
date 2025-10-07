#include "Drawing.h"
#include "UI.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    // AllocConsole();
    // freopen("CONOUT$", "w", stdout);  // std::cout
    // freopen("CONIN$", "r", stdin);    // std::cin

    Drawing::init();
    UI::Render();

    // FreeConsole();
    return oxorany(0);
}