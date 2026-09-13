#pragma once

class WindowState
{
public:
    int clientWidth = 0;
    int clientHeight = 0;
};

class Config
{
public:
	bool wallhack = false;
	bool chams = false;
	bool testVM = false;
	int wallhackKey = 0;
};

extern Config config;
