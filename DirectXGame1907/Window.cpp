#include "Window.h"

// �E�B���h�E�N���X
Window::WindowClass Window::WindowClass::windowClass;

// �R���X�g���N�^
Window::WindowClass::WindowClass() noexcept
	:
	hInstance( GetModuleHandle(nullptr) )
{
	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX wc = { 0 };
	// �\���̂̃T�C�Y
	wc.cbSize = sizeof( wc );
	// �E�B���h�E�X�^�C��	
	wc.style = 
		// CS_HREDRAW ���������̃T�C�Y�ύX���ɃE�B���h�E�S�̂��ĕ`�悷��
		CS_HREDRAW |
		// CS_VREDRAW ���������̃T�C�Y�ύX���ɃE�B���h�E�S�̂��ĕ`�悷��
		CS_VREDRAW;
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

// �f�X�X�g���N�^
Window::WindowClass::~WindowClass()
{
	// �E�B���h�E�̍폜
	UnregisterClass( windowClassName, GetInstance() );
}

// �E�B���h�E�N���X���̎擾
const char* Window::WindowClass::GetName() noexcept
{
	return windowClassName;
}

// �E�B���h�E�N���X�C���X�^���X�̎擾
HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return windowClass.hInstance;
}

// �R���X�g���N�^
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

	// Graphics�X�}�[�g�|�C���^
	pGfx = std::make_unique<Graphics>( hWnd );
}

// �f�X�X�g���N�^
Window::~Window()
{
	DestroyWindow( hWnd );
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

// �E�B���h�E�v���V�[�W��
LRESULT Window::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	switch ( msg )
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS:
		// �L�[�X�e�[�g�̃N���A
		keyboard.ClearState();
		break;

		// �L�[�{�[�h���b�Z�[�W
	case WM_KEYDOWN:

	case WM_SYSKEYDOWN:
		//�L�[���s�[�g�܂��̓L�[�{�[�h�̃L�[���s�[�g���L���Ȃ�
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

		// �}�E�X���b�Z�[�W
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