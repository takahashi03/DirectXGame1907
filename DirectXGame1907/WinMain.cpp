// Win32API
#include "Window.h"

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam )
{
	switch (msg)
	{		
	case WM_CLOSE:
		// �v���O�����̏I����ʒm����
		PostQuitMessage(0);
		break;
	}
	// �K��̃E�B���h�E�v���V�[�W�����Ăяo��
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// �G���g���|�C���g
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow )
{
	// �E�B���h�E�N���X�̖��O
	const auto pClassName = "DirectX11";

	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX wc = { 0 };
	// �\���̂̃T�C�Y
	wc.cbSize = sizeof(wc);
	// �E�B���h�E�X�^�C��
	// CS_HREDRAW ���������̃T�C�Y�ύX���ɃE�B���h�E�S�̂��ĕ`�悷��
	// CS_VREDRAW ���������̃T�C�Y�ύX���ɃE�B���h�E�S�̂��ĕ`�悷��
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// �E�C���h�E�̃��b�Z�[�W����������R�[���o�b�N�֐��ւ̃|�C���^���w��
	wc.lpfnWndProc = WndProc;
	// �E�B���h�E�N���X�̒ǉ��̈� �ʏ�0
	wc.cbClsExtra = 0;
	// �E�B���h�E�̒ǉ��̈� �ʏ�0
	wc.cbWndExtra = 0;
	// �A�C�R��
	wc.hIcon = nullptr;
	// �J�[�\��
	wc.hCursor = nullptr;
	// �w�i
	// GetStockObject ��`�ς݂̃u���V BLACK_BRUSH ���̃u���V
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	// �V�X�e���J���[
	// COLOR_ACTIVECAPTION �A�N�e�B�u�E�B���h�E�̃o�[�̐F
	// wc.hbrBackground = (HBRUSH)(COLOR_ACTIVECAPTION),
	// �f�t�H���g���j���[��
	wc.lpszMenuName = nullptr;
	// �E�B���h�E�N���X��
	wc.lpszClassName = pClassName;
	// �^�C�g���o�[�A�C�R��(16*16)
	wc.hIconSm = nullptr;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wc);

	// �E�B���h�E�̃C���X�^���X����	
	// CreateWindowEx �g���X�^�C�����g�p�ł���
	HWND hWnd = CreateWindowEx(
		// �g���X�^�C��
		0, 
		// �E�B���h�E�N���X��
		pClassName,
		// �^�C�g���o�[������
		"Window",
		// �E�B���h�E�X�^�C��
		// �^�C�g���o�[
		WS_CAPTION |
		// �ŏ����{�^��
		WS_MINIMIZEBOX |
		// �^�C�g���o�[�ɃE�B���h�E���j���[�{�b�N�X
		WS_SYSMENU,
		// �E�B���h�E�����X���W
		200,
		// �E�B���h�E�����Y���W
		200,
		// �E�B���h�E�̕�
		640,
		// �E�B���h�E�̍���
		480,
		// �e�E�B���h�E�̃n���h��
		nullptr,
		// ���j���[�n���h���܂��͎q��ID(���ʎq)
		nullptr,
		// �C���X�^���X�̃n���h��
		hInstance,
		// �E�B���h�E�̍쐬�f�[�^
		nullptr
	);

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, SW_SHOW);

	// ���b�Z�[�W���[�v
	MSG msg;
	BOOL gResult;
	// GetMessage ���b�Z�[�W�̎󂯎��
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		// ���b�Z�[�W���E�C���h�E�v���V�[�W����
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