#pragma once
#include "Window.h"

class App
{
public:
	App();
	// �}�X�^�\�t���[��
	int Go();
private:
	void DoFrame();
private:
	Window window;
};