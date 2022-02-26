/*==============================================================================

   [main.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include<Windows.h>
#include<d3dx9.h>
#include<stdio.h>
#include<time.h>
#include"mouse.h"
#include"Config.h"
#include"mydirect3D.h"
#include"texture.h"
#include"Facegen.h"
#include"sound.h"
#include"system_timer.h"
#include"keyboard.h"
#include"Keylogger.h"
#include"TextGen.h"
#include"Xmodel.h"
#include"Scene.h"
#include"Block.h"
#include"GameObjectClass.h"
#include"Fade.h"

#pragma comment (lib, "winmm.lib")

//*****************************************************************************
// �}�N��
//*****************************************************************************
#define CLASS_NAME		"GameWindow"	//�E�B���h�E�N���X�̖��O
#define WINDOW_CAPTION	"Let's Go Picnic!"	//�E�B���h�E�̖��O
//debug���[�h�����ɃR���\�[���Ăяo���d�g��
//#if defined(DEBUG) || defined(_DEBUG)
//#pragma comment(linker, "/subsystem:console")
//#define Main main
//#else
//#pragma comment(linker, "/subsystem:windows")
//#define Main _stdcall WinMain
//#endif
//*****************************************************************************
// �v���g�^�C�v�֐��錾
//*****************************************************************************

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);//window�v���V�[�W��
static bool Initialize(HWND hWnd);
static bool Update(void);
static void Draw(void);
static void Finalize(void);

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
static bool isContinueProgram = true;

//*****************************************************************************
// �֐���`
//*****************************************************************************
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdshow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//�E�B���h�E�N���X�\���̂̐ݒ�
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

	//�N���X�o�^
	RegisterClass(&wc);

	//�E�B���h�E�X�^�C��
	const DWORD WINDOW_STYLE = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);

	//�s���̂����E�B���h�E�T�C�Y�̎Z�o
	RECT window_rect = { 0,0,(LONG)SCREEN_WIDTH ,(LONG)SCREEN_HEIGHT };
	AdjustWindowRect(&window_rect, WINDOW_STYLE, FALSE);

	//�E�B���h�E�̋�`���畝�ƍ��������߂�
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	////�f�X�N�g�b�v�̃T�C�Y���擾
	RECT desktop_rect;
	GetWindowRect(GetDesktopWindow(), &desktop_rect);

	////�f�X�N�g�b�v�̕��ƍ������Z�o
	int desktop_width = desktop_rect.right - desktop_rect.left;
	int desktop_height = desktop_rect.bottom - desktop_rect.top;

	//�f�X�N�g�b�v�̐^�񒆂ɃE�B���h�E�����������悤�ɍ��W���v�Z
	int window_x = max((desktop_width - window_rect.right) / 2, 0);
	int window_y = max((desktop_height - window_rect.bottom) / 2, 0);

	//�E�B���h�E�̐���
	HWND hWnd = CreateWindow(//EX�ł�����A�t�@�C���̃h���b�O�A���h�h���b�v�Ƃ����g����
		CLASS_NAME,
		WINDOW_CAPTION,
		WINDOW_STYLE,

		//�T�C�Y�ƈʒu
		window_x,			//���j�^�[��ʍ��ォ��̋����@X
		window_y,			//���j�^�[��ʍ��ォ��̋����@Y
		window_width,		//��
		window_height,		//����

		NULL,		//�e�E�B���h�E�n���h���@�e����������q�������݂����Ȃ��Ƃ��ł���
		NULL,		//���j���[�n���h���@�Q�[�����͊O��
		hInstance,	//�C���X�^���X�n���h��
		NULL		//�ǉ��̃A�v���P�[�V�����f�[�^
	);

	if (hWnd == NULL)//�E�B���h�E�n���h�������炩�̗��R�Ő����ł��Ȃ������ꍇ
	{
		return 0;//�A�v���P�[�V�����I��
	}
	//�w��̃E�B���h�E�n���h���̃E�B���h�E���w��̕��@�ŕ\��
	ShowWindow(hWnd, nCmdshow);
	UpdateWindow(hWnd);

	//�Q�[���̏�����
	if (!Initialize(hWnd))
	{
		MessageBox(NULL, "�Q�[���̏������Ɏ��s���܂���", "�G���[", MB_OK);
		return 0;//�A�v���P�[�V�����I��
	}

	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>�Q�[���p���C�����[�v
	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//���b�Z�[�W������ꍇ�̓��b�Z�[�W������D��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				//�X�V
				if (!Update())break;

				SceneClass::ExecuteChangeScene();

				//�`��
				Draw();
			}
		}
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	timeEndPeriod(1);

	Finalize();
	return (int)msg.wParam;
}

//�E�B���h�E�v���V�[�W���i�R�[���o�b�N�֐��j	�t�b�N�@��肽��������D���ď㏑���H�@�@�f�B�X�p�b�`���Ă΂��Ƃ�����Ăԁ@���b�Z�[�W���Ƃɏ�������
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
	case WM_ACTIVATEAPP:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard_ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse_ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_CLOSE:
		if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	};

	return DefWindowProc(hWnd, uMsg, wParam, lParam);//�I���̏����͖���蓮�ŏ���
}

bool Initialize(HWND hWnd)
{
	isContinueProgram = true;

	srand((unsigned)time(NULL));

	Keyboard_Initialize();

	KeyloggerINIT();

	Mouse_Initialize(hWnd);

	InitSound(hWnd);

	//DirectX�̏�����
	if (!MyDirect3D_INIT(hWnd))
	{
		MessageBox(NULL, "Direct3D�̏������Ɏ��s���܂���", "�G���[", MB_OK);
		return false;
	}

	//�e�N�X�`���Ǘ����W���[���̏�����
	TextureINIT();

	//�X�v���C�g���W���[���̏�����
	FaceGenInit();

	TextGenInit();

	SystemTimer_Initialize();

	FadeClass::Init();

	GameObjectClass::Init();

	BlockBaseClass::Init();

	//g_Game.Init();

	SceneClass::Init();

	//�����_��BGM
	SOUND_LABEL temp;
	int temprand = rand() % 3;
	temp = temprand == 0 ? bgm1 :
		temprand == 1 ? bgm2 : bgm3;

	PlaySound(temp);

	return true;//����������
}

bool Update(void)
{
	KeyloggerUPDATE();

	TextGenUpdate();

	FadeClass::Update();

	//g_Game.Update();

	SceneClass::Update();

	return isContinueProgram;
}

void Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}

	//��ʃN���A
	//pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 240, 220, 255), 1.0f, 0);

	//�`��o�b�`���߂̊J�n
	pDevice->BeginScene();

	//g_Game.Draw();
	SceneClass::Draw();

	FadeClass::Draw();

	TextGenDrawforSecond();

	//�`��o�b�`���߂̏I��
	pDevice->EndScene();

	//�o�b�N�o�b�t�@���t���b�v�i�^�C�~���O��D3DPRESENT_PARAMETERS�̐ݒ�ɂ��j
	pDevice->Present(NULL, NULL, NULL, NULL);
}

void Finalize(void)
{
	//�I�������͏������̏��ԂƋt���ɍs��

	TextGenUninit();

	BlockBaseClass::Uninit();

	//g_Game.Uninit();
	SceneClass::Uninit();

	XModelReleaseAll();

	FaceGenUninit();

	TextureINIT();

	FadeClass::Uninit();

	MyDirect3D_FIN();

	UninitSound();

	KeyloggerUNINIT();

	TextureAllRelease();
}

void ProgramFinish(void)
{
	isContinueProgram = false;
}
