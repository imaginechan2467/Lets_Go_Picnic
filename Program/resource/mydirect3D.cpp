/*==============================================================================

   [myDirect3D.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include <d3d9.h>
#include"Config.h"

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
static LPDIRECT3D9 g_pD3D = NULL;//Direct3Dインターフェース
static PDIRECT3DDEVICE9 g_pDevice = NULL;//Direct3Dデバイスインターフェース


bool MyDirect3D_INIT(HWND hWnd)
{
	//Direct3Dインターフェースの取得
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		//Direct3Dインターフェースの取得に失敗
		MessageBox(NULL, "Direct3Dインターフェースの作成に失敗しました", "エラー", MB_OK);
		return false;
	}

	/*Direct3Dデバイスの取得*/

	//デバイスの取得に必要な情報構造体の作成
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.BackBufferWidth = (UINT)SCREEN_WIDTH;					//バックバッファの横幅　ダブルバッファの後ろ側？
	d3dpp.BackBufferHeight = (UINT)SCREEN_HEIGHT;				//バックバッファの縦幅
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;					//バックバッファのフォーマット　フルスクリーンにするときは気をつける部分
	d3dpp.BackBufferCount = 1;									//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//スワップ方法の設定　D3DSWAPEFFECT_DISCARDでだいたいOke
	d3dpp.Windowed = TRUE;										//ウィンドウ or フルスクリーン
	d3dpp.EnableAutoDepthStencil = TRUE;						//深度バッファ・ステンシルバッファの使用
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				//深度バッファステンシルバッファのフォーマット
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//フルスクリーン時のリフレッシュレートの指定
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//リフレッシュレートとPresent処理の関係	フルスクリーンなら30fpsとか60とかこっちが決めれる
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//リフレッシュレートとPresent処理の関係	フルスクリーンなら30fpsとか60とかこっちが決めれる

	//Direct3Dデバイスの取得
	HRESULT hr;
	hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice);//D3DCREATE_HARDWARE_VERTEXPROCESSINGはどこで頂点計算をするか（デバイスかソフトか）

	//HRESULT型変数は成功や失敗が値として入っているので失敗や成功だけ判定する場合はマクロを利用する　FAILED（hr） SUCCEEDED(hr)=成功してたら真
	if (FAILED(hr))//失敗が真
	{
		//デバイスの取得に失敗
		MessageBox(NULL, "Direct3Dデバイスの取得に失敗しました", "エラー", MB_OK);
		return false;
	}

	//UVアドレッシングモードの設定
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);//BORDER MIRROR CLAMP WRAP
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	//g_pDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_RGBA(255, 255, 255, 255));

	g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	g_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);

	//アルファブレンド
	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//アルファブレンドを有効化
	g_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//頂点カラーとテクスチャのブレンド設定
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//法線ベクトルは必ず正規化して使用する設定にする
	g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//αテスト
	/*g_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	g_pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	g_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/

	return true;//初期化成功
}


void MyDirect3D_FIN(void)
{
	if (g_pDevice)
	{
		g_pDevice->Release();
		g_pDevice = NULL;
	}
	if (g_pD3D)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void)
{
	return g_pDevice;
}