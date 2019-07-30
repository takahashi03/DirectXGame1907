#include "Window.h"

// ウィンドウクラス
Window::WindowClass Window::WindowClass::windowClass;

// コンストラクタ
Window::WindowClass::WindowClass() noexcept
	:
	hInstance( GetModuleHandle(nullptr) )
{
	// ウィンドウクラスの設定
	WNDCLASSEX wc = { 0 };
	// 構造体のサイズ
	wc.cbSize = sizeof( wc );
	// ウィンドウスタイル	
	wc.style = 
		// CS_HREDRAW 水平方向のサイズ変更時にウィンドウ全体を再描画する
		CS_HREDRAW |
		// CS_VREDRAW 垂直方向のサイズ変更時にウィンドウ全体を再描画する
		CS_VREDRAW;
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

// デスストラクタ
Window::WindowClass::~WindowClass()
{
	// ウィンドウの削除
	UnregisterClass( windowClassName, GetInstance() );
}

// ウィンドウクラス名の取得
const char* Window::WindowClass::GetName() noexcept
{
	return windowClassName;
}

// ウィンドウクラスインスタンスの取得
HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return windowClass.hInstance;
}

// コンストラクタ
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

	// Graphicsスマートポインタ
	pGfx = std::make_unique<Graphics>( hWnd );
}

// デスストラクタ
Window::~Window()
{
	DestroyWindow( hWnd );
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
	if ( msg == WM_NCCREATE )
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>( lParam );
		Window* const pWnd = static_cast<Window*>( pCreate->lpCreateParams );
		SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pWnd ) );
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( &Window::HandleMsgThunk) );
		return pWnd->WndProc( hWnd, msg, wParam, lParam );
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK Window::HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	Window* const pWnd = reinterpret_cast<Window*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
	return pWnd->WndProc( hWnd, msg, wParam, lParam );
}

// ウィンドウプロシージャ
LRESULT Window::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	switch ( msg )
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS:
		// キーステートのクリア
		keyboard.ClearState();
		break;

		// キーボードメッセージ
	case WM_KEYDOWN:

	case WM_SYSKEYDOWN:
		//キーリピートまたはキーボードのキーリピートが有効なら
		if (!(lParam & 0x40000000) || keyboard.IsAutoRepeatIsEnabled()) // filter autorepeat
		{
			keyboard.OnKeyDowned(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		keyboard.OnKeyUped(static_cast<unsigned char>(wParam));
		break;

	case WM_CHAR:
		keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;

		// マウスメッセージ
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);

		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow(hWnd);
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftDowned(pt.x, pt.y);		
		break;
	}
	case WM_RBUTTONDOWN:
	{
		SetForegroundWindow(hWnd);
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightDowned(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftUped(pt.x, pt.y);
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightUped(pt.x, pt.y);
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}