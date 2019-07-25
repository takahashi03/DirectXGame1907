#include "Graphics.h"

// DirectX11���C�u����
#pragma comment(lib,"d3d11.lib")

Graphics::Graphics(HWND hWnd)
{
	// �X���b�v�`�F�C��
	DXGI_SWAP_CHAIN_DESC sd = {};

	// �o�b�t�@���̐ݒ�
	sd.BufferCount = 1;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// MSAA�̐ݒ�
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	// �E�B���h�E�ݒ�
	sd.OutputWindow = hWnd;
	// �E�B���h�E�����������邩�H(false�Ńt���X�N���[��)
	sd.Windowed = TRUE;

	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// D3D11�f�o�C�X
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pDeviceContext
	);

	ID3D11Resource* pBackBuffer = nullptr;
	pSwapChain -> GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
	pDevice->CreateRenderTargetView(
		pBackBuffer,
		nullptr,
		&pTarget
	);

	pBackBuffer->Release();
}

Graphics::~Graphics()
{
	if ( pDeviceContext != nullptr )
	{
		pDeviceContext->Release();
	}
	if ( pSwapChain != nullptr )
	{
		pSwapChain->Release();
	}
	if ( pDevice != nullptr )
	{
		pDevice->Release();
	}
}

void Graphics::EndFrame()
{
	pSwapChain -> Present( 1u, 0u );
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red,green,blue,1.0f };
	pDeviceContext -> ClearRenderTargetView( pTarget, color );
}