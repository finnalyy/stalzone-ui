#pragma once

class Config
{
public:
	bool wallhack = false;
	bool chams = false;
	bool testVM = false;
	int wallhackKey = 0;
};

extern Config config;