//------------------------------------------
//
//�e�N�X�`���\���v���O�����̃��C���̏���[main.cpp]
//Author fuma sato
//
//------------------------------------------

#include"main.h"
#include"title.h"
#include"demo.h"
#include"tuto.h"
#include"game.h"
#include"result.h"
#include"rank.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
#include"option.h"
#include"cursor.h"
#include"vs.h"

#define IDI_MYICON "path_to_your_icon.ico"

LPDIRECT3D9 g_pD3D = NULL;//�_�C���N�gX�I�u�W�F�N�g�̃O���[�o���|�C���^��錾
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;//�_�C���N�gX�f�o�C�X�̃O���[�o���|�C���^��錾
MODE g_mode = MODE_TITLE;
HINSTANCE g_hInstance;
HWND g_hWnd;
bool g_bPause = false;
bool g_bFullScreen = false;

//------------------------
//���C���֐�
//------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	//�E�C���h�E�ݒ�
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),//�������T�C�Y
		CS_CLASSDC,//�X�^�C��
		WindowProc,//�v���V�[�W��
		0,//0
		0,//0
		hInstance,//�C���X�^���X�n���h��
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON)),//�^�X�N�o�[�A�C�R��
		LoadCursor(NULL,IDC_ARROW),//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 23),//�w�i�F
		NULL,//���j���[�o�[
		CLASS_NAME,//�N���X�̖��O
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON))//�A�C�R��
	};

	srand((int)time(NULL));//�����ݒ�
	HWND hWnd = nullptr;//�n���h��
	MSG msg = { 0 };//���b�Z�[�W
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };//�E�C���h�E�T�C�Y�̐ݒ�
	DWORD dwCurrntTime;//���݂̃V�X�e������
	DWORD dwExceLastTime;//�O��̃V�X�e������

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈�̃T�C�Y����
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx
	(
		0,//�X�^�C��
		CLASS_NAME,//�E�C���h�E�N���X�̖��O
		WINDOW_NAME,//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,//�X�^�C��
		CW_USEDEFAULT,//������W
		CW_USEDEFAULT,//�E�����W
		(rect.right - rect.left),//��
		(rect.bottom - rect.top),//����
		NULL,//�e
		NULL,//���j���[ID
		hInstance,//�C���X�^���X�n���h��
		NULL//�쐬�f�[�^
	);
	g_hInstance = hInstance;
	g_hWnd = hWnd;

	while (ShowCursor(FALSE) >= 0);
	ConfineCursorToWindow(hWnd);

	//�\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (FAILED(Init(hInstance, hWnd, TRUE)))//����������
	{
		return E_FAIL;
	}

	timeBeginPeriod(1);//����\�̐ݒ�
	dwCurrntTime = 0;//��������
	dwExceLastTime = timeGetTime();//�J�n�������L�^

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)//�I�����b�Z�[�W
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (!g_bPause)
			{
				dwCurrntTime = timeGetTime();//�������擾
				if ((dwCurrntTime - dwExceLastTime) >= DPS)//�ݒ肵���Ԋu���󂢂���
				{
					dwExceLastTime = dwCurrntTime;//����̍X�V�������L�^

					Update();//�X�V����
					Draw();//�`�揈��
				}
			}
		}
	}

	Uninit();//�I������

	//�E�C���h�E�N���X�̓o�^����
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	timeEndPeriod(1);//����\��߂�

	ReleaseCursor();
	while (ShowCursor(TRUE) < 0);

	//�I��
	return (int)msg.wParam;
}

//--------------------------
//�E�C���h�E�v���V�[�W��
//--------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID = 0;//�Ԃ�l�̊i�[
	static bool bFreeMause=false;

	switch (uMsg)
	{
	case WM_DESTROY://�j��
		//WM_QUIT���b�Z�[�W
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN://�L�[����
		switch (wParam)
		{
		case VK_ESCAPE://ESC�L�[
			ReleaseCursor();
			while (ShowCursor(TRUE) < 0);
			nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W....�����??...���Ԃ�...............�����炭...............", MB_YESNO | MB_ICONQUESTION);
			if (nID == IDYES)
			{
				//�j��(WM_DESTROY)
				DestroyWindow(hWnd);
			}
			else
			{
				while (ShowCursor(FALSE) >= 0);
				ConfineCursorToWindow(hWnd);
			}
			break;
		case VK_F4:
			if (bFreeMause)
			{
				bFreeMause = false;

				while (ShowCursor(FALSE) >= 0);
				ConfineCursorToWindow(hWnd);
			}
			else
			{
				bFreeMause = true;

				ReleaseCursor();
				while (ShowCursor(TRUE) < 0);
			}
			break;
		case VK_F5:
			CrCursor(!GetCursorIn()->bUse);
			break;
		case VK_F11:
			ToggleFullScreen(hWnd);
			break;
		}
		break;
	case WM_SETFOCUS:
		g_bPause = false;
		ConfineCursorToWindow(hWnd);
		break;
	case WM_KILLFOCUS:
		g_bPause = true;
		break;
	}
	//�K�v�f�[�^��Ԃ�
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//------------------------------
//����������
//------------------------------
HRESULT Init(HINSTANCE hInstanse, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;//�_�C���N�gX�f�B�X�v���C���[�h�̕ϐ��錾
	D3DPRESENT_PARAMETERS d3dpp;//�_�C���N�gX�p�����[�^�̕ϐ��錾

	//�I�u�W�F�N�g����
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//�f�B�X�v���C���[�h
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	ZeroMemory(&d3dpp, sizeof(d3dpp));//0��~���l�߂�

	//�o�b�N�o�b�t�@�̐ݒ�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//�f�o�C�X����
	if (FAILED(g_pD3D->CreateDevice
	(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice
	)))
	{
		if (FAILED(g_pD3D->CreateDevice
		(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice
		)))
		{
			if (FAILED(g_pD3D->CreateDevice
			(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice
			)))
			{
				return E_FAIL;
			}
		}
	}

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//�J�����O
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//�����x
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);


	//�I�u�W�F�N�g�̏���������
	if (FAILED(InitKeyboard(hInstanse, hWnd)))//�L�[����
	{
		return E_FAIL;
	}
	if (FAILED(InitMouse(hInstanse, hWnd)))//�L�[����
	{
		return E_FAIL;
	}
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}
	if (FAILED(InitdJoypad(hInstanse, hWnd)))//�L�[����
	{
		return E_FAIL;
	}
	//�T�E���h
	InitSound(hWnd);

	InitCursor();

	SetMode(g_mode);

	InitFade(g_mode);

	return S_OK;
}

//------------------------------
//�I������
//------------------------------
void Uninit(void)
{
	//�I�u�W�F�N�g�I��
	
	//�^�C�g�����
	UninitTitle();
	//�f�����
	UninitDemo();
	//�`���[�g���A�����
	UninitTuto();
	//�ΐ���
	UninitVs();
	//�Q�[�����
	UninitGame();
	//���U���g���
	UninitResult();
	//�����L���O���
	UninitRank();
	//�I�v�V����
	UninitOption();
	//�t�F�[�h
	UninitFade();
	//�T�E���h
	UninitSound();

	UninitCursor();//�J�[�\��
	UninitdJoypad();//d�p�b�h����
	UninitJoypad();//�p�b�h����
	UninitMouse();//�}�E�X����
	UninitKeyborad();//�L�[����

	//�f�o�C�X�j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//�I�u�W�F�N�g�j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//------------------------------
//�X�V����
//------------------------------
void Update(void)
{
	UpdateKeyborad();//�L�[����
	UpdateMouse();//�}�E�X����
	UpdateJoypad();//�p�b�h����
	UpdatedJoypad();//d�p�b�h����
	UpdateCursor();//�J�[�\��
	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();
		break;
	case MODE_DEMO:
		UpdateDemo();
		break;
	case MODE_TUTO:
		UpdateTuto();
		break;
	case MODE_GAME:
		UpdateGame();
		break;
	case MODE_VS:
		UpdateVs();
		break;
	case MODE_RESULT:
		UpdateResult();
		break;
	case MODE_RANK:
		UpdateRank();
		break;
	case MODE_OPTION:
		UpdateOption();
		break;
	}
	//�t�F�[�h
	UpdateFade();
}

//------------------------------
//�`�揈��
//------------------------------
void Draw(void)
{
	g_pD3DDevice->Clear//�t�����g�o�b�t�@�̃N���A
	(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0
	);

	if (SUCCEEDED(g_pD3DDevice->BeginScene()))//�`��J�n
	{
		//�I�u�W�F�N�g�`��
		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();
			break;
		case MODE_DEMO:
			DrawDemo();
			break;
		case MODE_TUTO:
			DrawTuto();
			break;
		case MODE_GAME:
			DrawGame();
			break;
		case MODE_VS:
			DrawVs();
			break;
		case MODE_RESULT:
			DrawResult();
			break;
		case MODE_RANK:
			DrawRank();
			break;
		case MODE_OPTION:
			DrawOption();
			break;
		}
		//�t�F�[�h
		DrawFade();
		//�J�[�\��
		DrawCursor();

		g_pD3DDevice->EndScene();//�`��I��
	}
	//�o�b�N�o�b�t�@�ɕ\����؂�ւ���
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//------------------------
//���[�h�̐؂�ւ�
//------------------------
void SetMode(MODE mode)
{
	//���݂̉�ʂ̏I������
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		break;
	case MODE_DEMO:
		UninitDemo();
		break;
	case MODE_TUTO:
		UninitTuto();
		break;
	case MODE_GAME:
		UninitGame();
		break;
	case MODE_VS:
		UninitVs();
		break;
	case MODE_RESULT:
		UninitResult();
		break;
	case MODE_RANK:
		UninitRank();
		break;
	case MODE_OPTION:
		UninitOption();
		break;
	}

	//�V������ʂ̏���������
	switch (mode)
	{
	case MODE_TITLE:
		InitTitle();
		break;
	case MODE_DEMO:
		InitDemo();
		break;
	case MODE_TUTO:
		InitTuto();
		break;
	case MODE_GAME:
		InitGame();
		break;
	case MODE_VS:
		InitVs();
		break;
	case MODE_RESULT:
		InitResult();
		break;
	case MODE_RANK:
		InitRank();
		break;
	case MODE_OPTION:
		InitOption();
		break;
	}

	g_mode = mode;
}

//------------------------
//�f�o�C�X�̎擾
//------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//-----------------
//���[�h�̎擾
//-----------------
MODE GetMode(void)
{
	return g_mode;
}

//-----------------
//�n���h���̎擾
//-----------------
HWND GethWnd(void)
{
	return g_hWnd;
}

//-----------------
//�C���X�^���X�n���h���擾
//-----------------
HINSTANCE GethInstanse(void)
{
	return g_hInstance;
}

//------------------
//�}�E�X����
//------------------
void ConfineCursorToWindow(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	MapWindowPoints(hWnd, NULL, (POINT*)&rect, 2);
	ClipCursor(&rect);
}

//-----------------
//�}�E�X���
//-----------------
void ReleaseCursor(void)
{
	ClipCursor(NULL); // ����������
}


//-------------------------------------------------
// �t���X�N���[���ƃE�B���h�E���[�h��؂�ւ���֐�
//-------------------------------------------------
void ToggleFullScreen(HWND hWnd)
{
	static WINDOWPLACEMENT wpPrev = { sizeof(wpPrev) };

	g_bFullScreen = !g_bFullScreen; // �t���O�𔽓]

	// �E�B���h�E�X�^�C����ύX
	if (g_bFullScreen)
	{
		GetWindowPlacement(hWnd,&wpPrev);

		SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOZORDER | SWP_FRAMECHANGED);

		//�f�B�X�v���C�ݒ��ύX
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = SCREEN_WIDTH;
		dmScreenSettings.dmPelsHeight = SCREEN_HEIGHT;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// �t���X�N���[���ݒ�Ɏ��s�����ꍇ�A�E�B���h�E���[�h�ɖ߂�
			SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPlacement(hWnd, &wpPrev);
			SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
			g_bFullScreen = false;
		}
	}
	else
	{
		ChangeDisplaySettings(NULL, 0);

		SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(hWnd, &wpPrev);
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	// �f�o�C�X���č쐬
	ResetDevice();
}

//----------------------------
// �f�o�C�X���č쐬����֐�
//----------------------------
void ResetDevice(void)
{
	// �f�o�C�X�����
	if (g_pD3DDevice != NULL)
	{
		//���Z�b�g�O�Ƀ��\�[�X�����
		UninitCursor();
		UninitOption();
		UninitRank();
		UninitResult();
		UninitVs();
		UninitGame();
		UninitTuto();
		UninitDemo();
		UninitTitle();
		UninitFade();
		UninitSound();

		// �f�o�C�X�����Z�b�g
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = !g_bFullScreen;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		HRESULT hr = g_pD3DDevice->Reset(&d3dpp);
		if (FAILED(hr))
		{
			MessageBox(NULL, "���߁[�[�[", "���ŁH�H�H�H�H�H�H�H�H", MB_OK | MB_ICONERROR);
			exit(1);
		}

		InitSound(g_hWnd);
		InitFade(g_mode);
		InitCursor();
	}
}