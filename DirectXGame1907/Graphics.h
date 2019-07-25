#pragma once
#include <Windows.h>
// DirectX11
#include <d3d11.h>

// 解放用テンプレート
template<class T>
void SefeRelease(T*& p)
{
	if(p)
	{
		p->Release();
		p = nullptr;
	}
}

class Graphics
{
public:
	Graphics(void);
	~Graphics(void);

	// 初期化
	bool Initialize(WinMain& window);
	// 終了処理
	void Finalize(void);

public:
	// デバイスの取得
	ID3D11Device * GetDevice(void) {}
};