#include "Window.h"
#include <sstream>

// �E�B���h�E�N���X
Window::WindowClass Window::WindowClass::windowClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInstance( GetModuleHandle(nullptr) )
{
	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX wc = { 0 };
	// �\���̂̃T�C�Y
	wc.cbSize = sizeof( wc );
	// �E�B���h�E�X�^�C��
	// CS_HREDRAW ���������̃T�C�Y�ύX���ɃE�B���h�E�S�̂��ĕ`�悷��
	// CS_VREDRAW ���������̃T�C�Y�ύX���ɃE�B���h�E�S�̂��ĕ`�悷��
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// �E�C���h�E�̃��b�Z�[�W����������R�[���o�b�N�֐��ւ̃|�C���^���w��
	wc.lpfnWndProc = HandleMsgSetup;
	// �E�B���h�E�N���X�̒ǉ��̈� �ʏ�0
	wc.cbClsExtra = 0;
	// �E�B���h�E�̒ǉ��̈� �ʏ�0
	wc.cbWndExtra = 0;
	// �A�C�R��
	wc.hIcon = nullptr;
	// �J�[�\��
	wc.hCursor = nullptr;
	// �w�i GetStockObject ��`�ς݂̃u���V BLACK_BRUSH ���̃u���V
	wc.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
	// �f�t�H���g���j���[��
	wc.lpszMenuName = nullptr;
	// �E�B���h�E�N���X��
	wc.lpszClassName = GetName();
	// �^�C�g���o�[�A�C�R��(16*16)
	wc.hIconSm = nullptr;

	// �E�B���h�E�N���X�̓o�^
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

// �E�B���h�E
Window::Window( int width, int height, const char* name ) :
	width( width ),
	height( height )
{
	// �T�C�Y�Ɋ�Â��ăE�B���h�E�T�C�Y���v�Z����
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	// �E�B���h�E���쐬����hWnd���擾
	//// CreateWindowEx �g���X�^�C�����g�p�ł���
	HWND hWnd = CreateWindow/*Ex*/(
		//// �g���X�^�C��
		//0,
        // �E�B���h�E�N���X��
		WindowClass::GetName(),
		// �^�C�g���o�[������
		name,
		// �E�B���h�E�X�^�C��
        // �^�C�g���o�[
		WS_CAPTION |
		// �ŏ����{�^��
		WS_MINIMIZEBOX |
		// �^�C�g���o�[�ɃE�B���h�E���j���[�{�b�N�X
		WS_SYSMENU,
		// �E�B���h�E�����X���W
		CW_USEDEFAULT,
		// �E�B���h�E�����Y���W
		CW_USEDEFAULT,
		// �E�B���h�E�̕�
		wr.right - wr.left,
		// �E�B���h�E�̍���
		wr.bottom - wr.top,
		// �e�E�B���h�E�̃n���h��
		nullptr,
		// ���j���[�n���h���܂��͎q��ID(���ʎq)
		nullptr,
		// �C���X�^���X�̃n���h��
		WindowClass::GetInstance(),
		// �E�B���h�E�̍쐬�f�[�^
		this
	);

	// �E�B���h�E�\��
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
	// ���b�Z�[�W���[�v
	MSG msg;
	
	while (PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE) )
	{		
		if ( msg.message == WM_QUIT )
		{
			return ( int )msg.wParam;
		}	
		// ���b�Z�[�W���E�C���h�E�v���V�[�W����
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
	// WinAPI���ŃE�B���h�E�N���X�|�C���^���i�[���邽�߂�CreateWindow�i�j����n���ꂽcreate�p�����[�^���g��
	if ( msg == WM_NCCREATE )
	{
		// �쐬�f�[�^����E�B���h�E�N���X�ւ�ptr�̒��o
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>( lParam );
		Window* const pWnd = static_cast<Window*>( pCreate->lpCreateParams );
		// �E�B���h�E�C���X�^���X��ptr���i�[����悤��WinAPI�Ǘ��̃��[�U�[�f�[�^��ݒ肵�܂��B
		SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pWnd ) );
		// �Z�b�g�A�b�v���I�������̂ŁA���b�Z�[�Wproc��ʏ�́i��Z�b�g�A�b�v�j�n���h���ɐݒ肵�܂�
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( &Window::HandleMsgThunk) );
		// �E�B���h�E�C���X�^���X�n���h���Ƀ��b�Z�[�W��]������
		return pWnd->HandleMsg( hWnd, msg, wParam, lParam );
	}
	// WM_NCCREATE���b�Z�[�W�̑O�Ƀ��b�Z�[�W���\�����ꂽ��A
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK Window::HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	// �E�B���h�E�C���X�^���X�ւ�ptr���擾����
	Window* const pWnd = reinterpret_cast<Window*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
	// �E�B���h�E�C���X�^���X�n���h���Ƀ��b�Z�[�W��]������
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