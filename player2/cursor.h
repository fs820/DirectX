//---------------------------------------
//
//カーソル処理の定義・宣言[cursor.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _CURSOR_H_
#define _CURSOR_H_

#include"main.h"

//プレイヤー情報
#define CURSOR_WIDTH (100)//幅
#define CURSOR_HEIGHT (100)//高さ
#define CURSOR_SPEED_DEF (0.0f)
#define CURSOR_SPEED_X (20.0f)//速さ
#define CURSOR_SPEED_Y (20.0f)

#define U_MAX_CU (18)//テクスチャの数(横)
#define V_MAX_CU (11)//テクスチャの数(縦)

typedef enum
{
	CURSORSTATE_NONE = 0,
	CURSORSTATE_NORMAL,
	CURSORSTATE_POINT,
	CURSORSTATE_MAX
}CURSORSTATE;

typedef struct
{
	int nCounterAnim;//プレイヤーの更新間隔
	int nPatternAnim;//プレイヤーのアニメーションパターン管理
	D3DXVECTOR3 pos;//プレイヤー座標管理
	D3DXVECTOR3 move;//プレイヤースピード管理
	CURSORSTATE state;//状態
	bool bUse;
}CURSOR;

void InitCursor(void);//ポリゴンの初期化処理
void UninitCursor(void);//ポリゴンの終了処理
void UpdateCursor(void);//ポリゴンの更新処理
void DrawCursor(void);//ポリゴンの描画処理
PDIRECT3DVERTEXBUFFER9 GetCursorBuffer(void);//プレイヤーバッファの取得
CURSOR* GetCursorIn(void);
void CrCursor(bool bUse);

#endif _CURSOR_H_
