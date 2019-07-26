#pragma once
#include <Windows.h>
// DirectX11�w�b�_�[
#include <d3d11.h>

class Graphics
{
public:
	// �R���X�g���N�^
	Graphics( HWND hWnd );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	// �f�X�X�g���N�^
	~Graphics();
	// �Ō�̃t���[��
	void EndFrame();
	// �N���A�o�b�t�@
	void ClearBuffer( float red, float green, float blue ) noexcept;
private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
};