#include "App.h"
#include "imgui/imgui.h"

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
	window.Gfx().ClearBuffer( 0.0f, 0.0f, 0.0f );
	window.Gfx().EndFrame();
}