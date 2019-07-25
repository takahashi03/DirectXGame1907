#include "App.h"

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
	const float c = sin( timer.Peek() ) / 2.0f + 0.5f;
	window.Gfx().ClearBuffer( c, c, 1.0f );
	window.Gfx().EndFrame();
}