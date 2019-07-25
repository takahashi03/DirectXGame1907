// Win32API
#include "Window.h"

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam )
{
	switch (msg)
	{		
	case WM_CLOSE:
		// プログラムの終了を通知する
		PostQuitMessage(0);
		break;
	}
	// 規定のウィンドウプロシージャを呼び出す
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// エントリポイント
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow )
{
	// ウィンドウクラスの名前
	const auto pClassName = "DirectX11";

	// ウィンドウクラスの設定
	WNDCLASSEX wc = { 0 };
	// 構造体のサイズ
	wc.cbSize = sizeof(wc);
	// ウィンドウスタイル
	// CS_HREDRAW 水平方向のサイズ変更時にウィンドウ全体を再描画する
	// CS_VREDRAW 垂直方向のサイズ変更時にウィンドウ全体を再描画する
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// ウインドウのメッセージを処理するコールバック関数へのポインタを指定
	wc.lpfnWndProc = WndProc;
	// ウィンドウクラスの追加領域 通常0
	wc.cbClsExtra = 0;
	// ウィンドウの追加領域 通常0
	wc.cbWndExtra = 0;
	// アイコン
	wc.hIcon = nullptr;
	// カーソル
	wc.hCursor = nullptr;
	// 背景
	// GetStockObject 定義済みのブラシ BLACK_BRUSH 黒のブラシ
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	// システムカラー
	// COLOR_ACTIVECAPTION アクティブウィンドウのバーの色
	// wc.hbrBackground = (HBRUSH)(COLOR_ACTIVECAPTION),
	// デフォルトメニュー名
	wc.lpszMenuName = nullptr;
	// ウィンドウクラス名
	wc.lpszClassName = pClassName;
	// タイトルバーアイコン(16*16)
	wc.hIconSm = nullptr;

	// ウィンドウクラスの登録
	RegisterClassEx(&wc);

	// ウィンドウのインスタンス生成	
	// CreateWindowEx 拡張スタイルを使用できる
	HWND hWnd = CreateWindowEx(
		// 拡張スタイル
		0, 
		// ウィンドウクラス名
		pClassName,
		// タイトルバー文字列
		"Window",
		// ウィンドウスタイル
		// タイトルバー
		WS_CAPTION |
		// 最小化ボタン
		WS_MINIMIZEBOX |
		// タイトルバーにウィンドウメニューボックス
		WS_SYSMENU,
		// ウィンドウ左上のX座標
		200,
		// ウィンドウ左上のY座標
		200,
		// ウィンドウの幅
		640,
		// ウィンドウの高さ
		480,
		// 親ウィンドウのハンドル
		nullptr,
		// メニューハンドルまたは子のID(識別子)
		nullptr,
		// インスタンスのハンドル
		hInstance,
		// ウィンドウの作成データ
		nullptr
	);

	// ウィンドウの表示
	ShowWindow(hWnd, SW_SHOW);

	// メッセージループ
	MSG msg;
	BOOL gResult;
	// GetMessage メッセージの受け取り
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		// メッセージをウインドウプロシージャへ
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == 1)
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
}