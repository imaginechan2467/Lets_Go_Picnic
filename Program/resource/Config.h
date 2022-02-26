#ifndef CONFIG_H_
#define CONFIG_H_

#include<d3dx9.h>

//DISPLAY SIZE
#define SCREEN_WIDTH  (1280.0f)
#define SCREEN_HEIGHT (720.0f)

//COLOR

enum COLORLIST
{
	colorlist_white,
	colorlist_black,
	colorlist_red,
	colorlist_green,
	colorlist_blue,
	colorlist_yellow,
	colorlist_grange,
	colorlist_lightblue,
	colorlist_lightgreen,
	colorlist_pink,
	colorlist_purple,
	color_MAX,
};

#define COLOR_NORMAL (D3DCOLOR_RGBA(255,255,255,255))	//白 & テクスチャカラー
#define COLOR_BLACK  (D3DCOLOR_RGBA(0,0,0,255))			//黒
#define COLOR_RED    (D3DCOLOR_RGBA(255,0,0,255))		//赤
#define COLOR_GREEN  (D3DCOLOR_RGBA(0,255,0,255))		//緑
#define COLOR_BLUE   (D3DCOLOR_RGBA(0,0,255,255))		//青
#define COLOR_YELLOW (D3DCOLOR_RGBA(255,255,0,255))		//黄色
#define COLOR_ORANGE (D3DCOLOR_RGBA(255,128,0,255))		//オレンジ
#define COLOR_LIGHTBLUE (D3DCOLOR_RGBA(0,128,255,255))	//水色
#define COLOR_LIGHTGREEN (D3DCOLOR_RGBA(0,255,128,255))	//黄緑色
#define COLOR_PINK (D3DCOLOR_RGBA(255,128,128,255))		//ピンク
#define COLOR_PURPLE (D3DCOLOR_RGBA(255,0,255,255))		//紫

#define COLOR_GREEN_GRID (D3DCOLOR_RGBA(0,128,0,255))		//グリッドグリーン

#define COLOR_WHITE_DEBUG      (D3DCOLOR_RGBA(255,255,255,128))	//デバッグ用の薄い白色
#define COLOR_YELLOW_DEBUG     (D3DCOLOR_RGBA(255,255,0  ,128))	//デバッグ用黄色
#define COLOR_ORANGE_DEBUG     (D3DCOLOR_RGBA(255,128,0  ,128))	//デバッグ用オレンジ
#define COLOR_LIGHTBLUE_DEBUG  (D3DCOLOR_RGBA(0  ,128,255,128))	//デバッグ用水色
#define COLOR_LIGHTGREEN_DEBUG (D3DCOLOR_RGBA(0  ,255,128,128))	//デバッグ用黄緑色
#define COLOR_PINK_DEBUG       (D3DCOLOR_RGBA(255,128,128,128))	//デバッグ用ピンク
#define COLOR_PURPLE_DEBUG     (D3DCOLOR_RGBA(255,0  ,255,128))	//デバッグ用紫
#define COLOR_BLACK_DEBUG      (D3DCOLOR_RGBA(0  ,0  ,0  ,128))	//デバッグ用黒
#define COLOR_RED_DEBUG        (D3DCOLOR_RGBA(255,0  ,0  ,128))	//デバッグ用赤
#define COLOR_GREEN_DEBUG      (D3DCOLOR_RGBA(0  ,255,0  ,128))	//デバッグ用緑
#define COLOR_BLUE_DEBUG       (D3DCOLOR_RGBA(0  ,0  ,255,128))	//デバッグ用青

//POS
#define CENTERPOS(SIZE)(vec2f(SCREEN_WIDTH / 2 - SIZE.x / 2,SCREEN_HEIGHT / 2 - SIZE.y / 2))//画面真ん中
#define vec2f_NULL (vec2f(0,0))
#define vec3f_NULL (vec3f(0,0,0))

//TEXTURE
#define TEXTUREALLDISP (MakeInt2(1,1))//テクスチャを一枚のポリゴンで全表示する

//SIZE
#define DEBUGTEXTSIZE (vec2f(24,24))
#define SCREENSIZE (vec2f(SCREEN_WIDTH,SCREEN_HEIGHT))

//名前省略
#define vec2f D3DXVECTOR2
#define vec3f D3DXVECTOR3

//変換系
#define vec3_to_vec2(v3)(vec2f(v3.x,v3.y))
#define vec2_to_vec3(v2)(vec3f(v2.x,v2.y,0.0f))

//エラー系
#define ERROR_NUMBER (-1)

//方向指定に使用
enum DIR
{
	dir_top,
	dir_under,
	dir_right,
	dir_left,

	dir_MAX,
};

enum DIRfor3D
{
	dir3_top,//yUP
	dir3_under,//yDOWN
	dir3_right,
	dir3_left,
	dir3_forward,//z前
	dir3_back,//z後ろ

	dir3_MAX,
};

enum ADD_SUB
{
	sub = -1,
	add = 1,
};

//テクスチャ切り取り指定時に使用
typedef struct
{
	int x;
	int y;
}Int2;
Int2 MakeInt2(int x, int y);

typedef vec2f Float2;
typedef vec3f Float3;

typedef struct
{
	int x;
	int y;
	int z;
}Int3;
Int3 MakeInt3(int x, int y,int z);

#endif
