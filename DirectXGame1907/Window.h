#pragma once
#include <Windows.h>

class Window
{
private:
	// �V���O���g���̓E�B���h�E�N���X�̓o�^ / �N���[���A�b�v���Ǘ�
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		static constexpr const char* windowClassName = "window";
		static WindowClass windowClass;
		HINSTANCE hInstance;
	};
public:
	// �R���X�g���N�^
	Window(int width, int height, const char* name);
	// �f�X�X�g���N�^
	~Window();
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	int width;
	int height;
	HWND hWnd;
};

