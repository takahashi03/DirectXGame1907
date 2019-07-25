#pragma once
#include <Windows.h>
// DirectX11ÉwÉbÉ_Å[
#include <d3d11.h>

class Graphics
{
public:
	Graphics( HWND hWnd );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	~Graphics();
	void EndFrame();
	void ClearBuffer( float red, float green, float blue ) noexcept;
private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
};