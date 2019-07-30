#include "App.h"
#include "imgui/imgui.h"

App::App()
	:
	window(800, 600, "Title")
{}

// マスタ―フレーム
int App::Go()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			// 値がある場合は終了コードを返す
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