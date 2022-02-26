/*==============================================================================

   [FaceGen.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------

	memo:

==============================================================================*/

#ifndef FACEGEN_H_
#define FACEGEN_H_

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include "Config.h"
#pragma warning(disable:26812)
//*****************************************************************************
// 列挙型宣言
//*****************************************************************************
enum GAGEMODE
{
	gage_add,
	gage_sub,
};

//*****************************************************************************
// プロトタイプ関数宣言
//*****************************************************************************

enum FACEGENTYPE
{
	facegentype_center,
	facegentype_leftup,

	facegentype_MAX,
};

void FaceGenInit();
void FaceGenUninit();

//ノーマル
void FaceGen(vec2f pos, vec2f size, bool IsUseTex,
	int TexId, D3DCOLOR Color = COLOR_NORMAL);

//シンプルポリゴン表示
void FaceGenSimple(vec2f pos, vec2f size,
	D3DCOLOR Color = COLOR_NORMAL);

void FaceGen3D(vec3f pos, vec2f scale, FACEGENTYPE typebool,
	bool IsUseTex, int TexId, float radian = 0.0f, D3DCOLOR Color = COLOR_NORMAL);

void FaceGenforTex3D(vec3f pos, vec2f scale, FACEGENTYPE type, bool IsUseTex,
	int TexId, int CurrentFrame, float radian = 0.0f, D3DCOLOR Color = COLOR_NORMAL);

//テキスチャ切り取り
void FaceGenforTex(vec2f pos, vec2f size, bool IsUseTex, int TexId,
	int CurrentFrame, D3DCOLOR Color = COLOR_NORMAL);

//テクスチャ切り取り && 方向反転モード付き。
//第7引数にdir_leftを入れた場合、テクスチャのXを反転する。
void FaceGenforTexInvert(vec2f pos, vec2f size, bool IsUseTex, int TexId,
	int CurrentFrame, DIR dir, D3DCOLOR Color = COLOR_NORMAL);

//回転
void FaceGenRotate(vec2f pos, vec2f size, bool IsUseTex, int TexId,
	int CurrentFrame, float angle, D3DCOLOR Color = COLOR_NORMAL);

//ゲージ
void FaceGenforGage(vec2f pos, vec2f size, float GageNum, DIR dir,
	GAGEMODE mode, bool isUI, D3DCOLOR Color = COLOR_NORMAL);

//円
//追加：ハイクオリティモード、HQの場合Rに合わせて高精度vertexの作り出す(性能悪いので使わない方がいい)
void CircleGen(vec2f pos, float R, int vertexNum, D3DCOLOR color);

//円ライン
void CircleGenForLine(vec2f pos, float R, D3DCOLOR color = COLOR_NORMAL);

//四角のライン。スポンジボブ、ズボンは四角。
void LineGenforSquare(vec2f pos, vec2f size, D3DCOLOR Color = COLOR_NORMAL);

void LineGenforSquare3D(vec2f pos, vec2f size, D3DCOLOR Color = COLOR_NORMAL);

//一本のラインを作成。
void LineGen(vec2f startpos, vec2f endpos, D3DCOLOR Color = COLOR_NORMAL);

void LineGen3D(vec2f startpos, float size, DIR dir, D3DCOLOR Color = COLOR_NORMAL);

//三角形。
void DeltaGen(vec2f pos, vec2f size, DIR dir, D3DCOLOR Color = COLOR_NORMAL);

void CircleLightGen(vec2f pos, float R, unsigned Intensity, D3DCOLOR Color);
//レンダリングバッファ専用。
void FaceGenRenderRotate(vec2f pos, vec2f size,
	IDirect3DTexture9* Render, float w, float h,
	float angel = 0.f, D3DCOLOR Color = COLOR_NORMAL);

void RingGen(vec2f pos, float R1, float R2, D3DCOLOR color1, D3DCOLOR color2, int Prec = 5);
#endif