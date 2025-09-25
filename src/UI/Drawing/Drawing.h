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

    static void init();
};