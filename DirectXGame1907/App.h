#pragma once
#include "Window.h"

class App
{
public:
	App();
	// マスタ―フレーム
	int Go();
private:
	void DoFrame();
private:
	Window window;
};