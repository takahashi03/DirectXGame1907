#pragma once
#include <Windows.h>
// DirectX11
#include <d3d11.h>

// ����p�e���v���[�g
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

	// ������
	bool Initialize(WinMain& window);
	// �I������
	void Finalize(void);

public:
	// �f�o�C�X�̎擾
	ID3D11Device * GetDevice(void) {}
};