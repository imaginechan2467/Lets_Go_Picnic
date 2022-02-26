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
// マクロ
//*****************************************************************************
#define CLASS_NAME		"GameWindow"	//ウィンドウクラスの名前
#define WINDOW_CAPTION	"Let's Go Picnic!"	//ウィンドウの名前
//debugモードだけにコンソール呼び出す仕組み
//#if defined(DEBUG) || defined(_DEBUG)
//#pragma comment(linker, "/subsystem:console")
//#define Main main
//#else
//#pragma comment(linker, "/subsystem:windows")
//#define Main _stdcall WinMain
//#endif
//*****************************************************************************
// プロトタイプ関数宣言
//*****************************************************************************

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);//windowプロシージャ
static bool Initialize(HWND hWnd);
static bool Update(void);
static void Draw(void);
static void Finalize(void);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
static bool isContinueProgram = true;

//*****************************************************************************
// 関数定義
//*****************************************************************************
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdshow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//ウィンドウクラス構造体の設定
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

	//クラス登録
	RegisterClass(&wc);

	//ウィンドウスタイル
	const DWORD WINDOW_STYLE = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);

	//都合のいいウィンドウサイズの算出
	RECT window_rect = { 0,0,(LONG)SCREEN_WIDTH ,(LONG)SCREEN_HEIGHT };
	AdjustWindowRect(&window_rect, WINDOW_STYLE, FALSE);

	//ウィンドウの矩形から幅と高さを求める
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	////デスクトップのサイズを取得
	RECT desktop_rect;
	GetWindowRect(GetDesktopWindow(), &desktop_rect);

	////デスクトップの幅と高さを算出
	int desktop_width = desktop_rect.right - desktop_rect.left;
	int desktop_height = desktop_rect.bottom - desktop_rect.top;

	//デスクトップの真ん中にウィンドウが生成されるように座標を計算
	int window_x = max((desktop_width - window_rect.right) / 2, 0);
	int window_y = max((desktop_height - window_rect.bottom) / 2, 0);

	//ウィンドウの生成
	HWND hWnd = CreateWindow(//EX版もあり、ファイルのドラッグアンドドロップとかが使える
		CLASS_NAME,
		WINDOW_CAPTION,
		WINDOW_STYLE,

		//サイズと位置
		window_x,			//モニター画面左上からの距離　X
		window_y,			//モニター画面左上からの距離　Y
		window_width,		//幅
		window_height,		//高さ

		NULL,		//親ウィンドウハンドル　親が動いたら子も動くみたいなことができる
		NULL,		//メニューハンドル　ゲーム時は外す
		hInstance,	//インスタンスハンドル
		NULL		//追加のアプリケーションデータ
	);

	if (hWnd == NULL)//ウィンドウハンドルが何らかの理由で生成できなかった場合
	{
		return 0;//アプリケーション終了
	}
	//指定のウィンドウハンドルのウィンドウを指定の方法で表示
	ShowWindow(hWnd, nCmdshow);
	UpdateWindow(hWnd);

	//ゲームの初期化
	if (!Initialize(hWnd))
	{
		MessageBox(NULL, "ゲームの初期化に失敗しました", "エラー", MB_OK);
		return 0;//アプリケーション終了
	}

	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>ゲーム用メインループ
	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//メッセージがある場合はメッセージ処理を優先
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				//更新
				if (!Update())break;

				SceneClass::ExecuteChangeScene();

				//描画
				Draw();
			}
		}
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	timeEndPeriod(1);

	Finalize();
	return (int)msg.wParam;
}

//ウィンドウプロシージャ（コールバック関数）	フック　やりたい処理を奪って上書き？　　ディスパッチが呼ばれるとこれを呼ぶ　メッセージごとに処理する
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
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	};

	return DefWindowProc(hWnd, uMsg, wParam, lParam);//終わりの処理は毎回手動で書く
}

bool Initialize(HWND hWnd)
{
	isContinueProgram = true;

	srand((unsigned)time(NULL));

	Keyboard_Initialize();

	KeyloggerINIT();

	Mouse_Initialize(hWnd);

	InitSound(hWnd);

	//DirectXの初期化
	if (!MyDirect3D_INIT(hWnd))
	{
		MessageBox(NULL, "Direct3Dの初期化に失敗しました", "エラー", MB_OK);
		return false;
	}

	//テクスチャ管理モジュールの初期化
	TextureINIT();

	//スプライトモジュールの初期化
	FaceGenInit();

	TextGenInit();

	SystemTimer_Initialize();

	FadeClass::Init();

	GameObjectClass::Init();

	BlockBaseClass::Init();

	//g_Game.Init();

	SceneClass::Init();

	//ランダムBGM
	SOUND_LABEL temp;
	int temprand = rand() % 3;
	temp = temprand == 0 ? bgm1 :
		temprand == 1 ? bgm2 : bgm3;

	PlaySound(temp);

	return true;//初期化成功
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

	//画面クリア
	//pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 240, 220, 255), 1.0f, 0);

	//描画バッチ命令の開始
	pDevice->BeginScene();

	//g_Game.Draw();
	SceneClass::Draw();

	FadeClass::Draw();

	TextGenDrawforSecond();

	//描画バッチ命令の終了
	pDevice->EndScene();

	//バックバッファをフリップ（タイミングはD3DPRESENT_PARAMETERSの設定による）
	pDevice->Present(NULL, NULL, NULL, NULL);
}

void Finalize(void)
{
	//終了処理は初期化の順番と逆順に行う

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
