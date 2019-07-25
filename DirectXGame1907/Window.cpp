#include "Window.h"

// �E�B���h�E�N���X
Window::WindowClass Window::WindowClass::windowClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}
Window::WindowClass::~WindowClass()
{
	UnregisterClass(windowClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return windowClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return windowClass.hInstance;
}

// �E�B���h�E
Window::Window(int width, int height, const char* name)
	:
	width(width),
	height(height)
{
	// �T�C�Y�Ɋ�Â��ăE�B���h�E�T�C�Y���v�Z����
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	// �E�B���h�E���쐬����hWnd���擾
	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	// �E�B���h�E�\���H
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// WinAPI���ŃE�B���h�E�N���X�|�C���^���i�[���邽�߂�CreateWindow�i�j����n���ꂽcreate�p�����[�^���g��
	if (msg == WM_NCCREATE)
	{
		// �쐬�f�[�^����E�B���h�E�N���X�ւ�ptr�̒��o
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// �E�B���h�E�C���X�^���X��ptr���i�[����悤��WinAPI�Ǘ��̃��[�U�[�f�[�^��ݒ肵�܂��B
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// �Z�b�g�A�b�v���I�������̂ŁA���b�Z�[�Wproc��ʏ�́i��Z�b�g�A�b�v�j�n���h���ɐݒ肵�܂�
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// �E�B���h�E�C���X�^���X�n���h���Ƀ��b�Z�[�W��]������
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// WM_NCCREATE���b�Z�[�W�̑O�Ƀ��b�Z�[�W���\�����ꂽ��A
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// �E�B���h�E�C���X�^���X�ւ�ptr���擾����
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// �E�B���h�E�C���X�^���X�n���h���Ƀ��b�Z�[�W��]������
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}