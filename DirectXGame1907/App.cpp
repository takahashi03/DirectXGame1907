#include "App.h"

App::App()
	:
	window(800, 600, "Title")
{}

// �}�X�^�\�t���[��
int App::Go()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			// �l������ꍇ�͏I���R�[�h��Ԃ�
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	const float c = sin( timer.Peek() ) / 2.0f + 0.5f;
	window.Gfx().ClearBuffer( c, c, 1.0f );
	window.Gfx().EndFrame();
}