#include "App.h"

// �G���g���|�C���g
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	App{}.Go();	

	return -1;
}