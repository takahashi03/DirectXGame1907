#pragma once
#include "Window.h"
#include "MyTimer.h"

class App
{
public:
	App();
	// �}�X�^�\�t���[�� / ���b�Z�[�W���[�v
	int Go();
private:
	void DoFrame();
private:
	Window window;
	MyTimer timer;
};