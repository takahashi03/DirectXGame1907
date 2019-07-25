#include "Window.h"
#include <sstream>

// ウィンドウクラス
Window::WindowClass Window::WindowClass::windowClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInstance( GetModuleHandle(nullptr) )
{
	// ウィンドウクラスの設定
	WNDCLASSEX wc = { 0 };
	// 構造体のサイズ
	wc.cbSize = sizeof( wc );
	// ウィンドウスタイル
	// CS_HREDRAW 水平方向のサイズ変更時にウィンドウ全体を再描画する
	// CS_VREDRAW 垂直方向のサイズ変更時にウィンドウ全体を再描画する
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// ウインドウのメッセージを処理するコールバック関数へのポインタを指定
	wc.lpfnWndProc = HandleMsgSetup;
	// ウィンドウクラスの追加領域 通常0
	wc.cbClsExtra = 0;
	// ウィンドウの追加領域 通常0
	wc.cbWndExtra = 0;
	// アイコン
	wc.hIcon = nullptr;
	// カーソル
	wc.hCursor = nullptr;
	// 背景 GetStockObject 定義済みのブラシ BLACK_BRUSH 黒のブラシ
	wc.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
	// デフォルトメニュー名
	wc.lpszMenuName = nullptr;
	// ウィンドウクラス名
	wc.lpszClassName = GetName();
	// タイトルバーアイコン(16*16)
	wc.hIconSm = nullptr;

	// ウィンドウクラスの登録
	RegisterClassEx( &wc );
}
Window::WindowClass::~WindowClass()
{
	UnregisterClass( windowClassName, GetInstance() );
}

const char* Window::WindowClass::GetName() noexcept
{
	return windowClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return windowClass.hInstance;
}

// ウィンドウ
Window::Window( int width, int height, const char* name ) :
	width( width ),
	height( height )
{
	// サイズに基づいてウィンドウサイズを計算する
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	// ウィンドウを作成してhWndを取得
	//// CreateWindowEx 拡張スタイルを使用できる
	HWND hWnd = CreateWindow/*Ex*/(
		//// 拡張スタイル
		//0,
        // ウィンドウクラス名
		WindowClass::GetName(),
		// タイトルバー文字列
		name,
		// ウィンドウスタイル
        // タイトルバー
		WS_CAPTION |
		// 最小化ボタン
		WS_MINIMIZEBOX |
		// タイトルバーにウィンドウメニューボックス
		WS_SYSMENU,
		// ウィンドウ左上のX座標
		CW_USEDEFAULT,
		// ウィンドウ左上のY座標
		CW_USEDEFAULT,
		// ウィンドウの幅
		wr.right - wr.left,
		// ウィンドウの高さ
		wr.bottom - wr.top,
		// 親ウィンドウのハンドル
		nullptr,
		// メニューハンドルまたは子のID(識別子)
		nullptr,
		// インスタンスのハンドル
		WindowClass::GetInstance(),
		// ウィンドウの作成データ
		this
	);

	// ウィンドウ表示
	ShowWindow( hWnd, SW_SHOWDEFAULT );

	pGfx = std::make_unique<Graphics>( hWnd );
}

Window::~Window()
{
	DestroyWindow( hWnd );
}

void Window::SetTitle(const std::string& title)
{
}

std::optional<int> Window::ProcessMessages()
{
	// メッセージループ
	MSG msg;
	
	while (PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE) )
	{		
		if ( msg.message == WM_QUIT )
		{
			return ( int )msg.wParam;
		}	
		// メッセージをウインドウプロシージャへ
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return {};
}

Graphics & Window::Gfx()
{
	return *pGfx;
}

LRESULT CALLBACK Window::HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	// WinAPI側でウィンドウクラスポインタを格納するためにCreateWindow（）から渡されたcreateパラメータを使う
	if ( msg == WM_NCCREATE )
	{
		// 作成データからウィンドウクラスへのptrの抽出
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>( lParam );
		Window* const pWnd = static_cast<Window*>( pCreate->lpCreateParams );
		// ウィンドウインスタンスにptrを格納するようにWinAPI管理のユーザーデータを設定します。
		SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pWnd ) );
		// セットアップが終了したので、メッセージprocを通常の（非セットアップ）ハンドラに設定します
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( &Window::HandleMsgThunk) );
		// ウィンドウインスタンスハンドラにメッセージを転送する
		return pWnd->HandleMsg( hWnd, msg, wParam, lParam );
	}
	// WM_NCCREATEメッセージの前にメッセージが表示されたら、
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK Window::HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	// ウィンドウインスタンスへのptrを取得する
	Window* const pWnd = reinterpret_cast<Window*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
	// ウィンドウインスタンスハンドラにメッセージを転送する
	return pWnd->HandleMsg( hWnd, msg, wParam, lParam );
}

LRESULT Window::HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	switch ( msg )
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}