#pragma once
#include <Windows.h>
#include <optional>
#include <memory>
#include "Graphics.h"

class Window
{
	// ウィンドウクラスの登録 / クリーンアップを管理
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass( const WindowClass& ) = delete;
		WindowClass& operator=( const WindowClass& ) = delete;
		static constexpr const char* windowClassName = "Window";
		static WindowClass windowClass;
		HINSTANCE hInstance;
	};

public:
	Window( int width, int height, const char* name );
	~Window();
	Window( const Window& ) = delete;
	Window& operator = ( const Window& ) = delete;
	static std::optional<int> ProcessMessages();
	Graphics& Gfx();
private:
	static LRESULT CALLBACK HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	static LRESULT CALLBACK HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	LRESULT WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
private:
	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
};