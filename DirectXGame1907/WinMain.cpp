#include "App.h"

// エントリポイント
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	// マスタ―フレーム
	App{}.Go();	
	return -1;
}