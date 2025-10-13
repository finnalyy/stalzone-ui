#pragma once

struct MenuState {
	int selectedTab = 0;
};

MenuState menuState;

struct KeyBindState {
    bool listening = false;
    int temp_bind = 0;
};