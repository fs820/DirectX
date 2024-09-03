//---------------------------------------
//
//プレイヤー表示処理の定義・宣言[player.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include"main.h"

//プレイヤー情報
#define PLAYER_WIDTH (160)//幅
#define PLAYER_HEIGHT (90)//高さ
#define PLAYER_SPEED_DEF (0.0f)
#define PLAYER_SPEED_X (3.0f)//速さ
#define PLAYER_SPEED_Y (3.0f)
#define PLAYER_INA (0.2f)
#define PLAYER_ROT_SPEED (0.05f)
#define PLAYER_SIZE_SPEED (3.0f)
#define PLAYER_SIZE_MAX (300.0f)
#define PLAYER_SIZE_MIN (5.0f)

#define U_MAX (1)//テクスチャの数(横)
#define V_MAX (1)//テクスチャの数(縦)

typedef enum
{
	PLAYERSTATE_APPEAR = 0,
	PLAYERSTATE_NORMAL,
	PLAYERSTATE_DAMAGE,
	PLAYERSTATE_DIE,
	PLAYERSTATE_MAX
}PLAYERSTATE;

typedef struct
{
	int nCounterAnim;//プレイヤーの更新間隔
	int nPatternAnim;//プレイヤーのアニメーションパターン管理
	D3DXVECTOR3 pos;//プレイヤー座標管理
	D3DXVECTOR3 move;//プレイヤースピード管理
	D3DXVECTOR3 rot;//プレイヤー向き管理
	D3DXVECTOR3 rotDef;//プレイヤー向き管理
	float fLength;//対角線の長さ(大きさ)
	float fLengthDef;//対角線の長さ(大きさ)の初期値
	float fAngle;//対角線の角度
	int nRema;
	int nLife;//体力
	PLAYERSTATE state;//状態
	int nCounterState;
	bool bDisp;
}Player;

void InitPlayer(void);//ポリゴンの初期化処理
void UninitPlayer(void);//ポリゴンの終了処理
void UpdatePlayer(void);//ポリゴンの更新処理
void DrawPlayer(void);//ポリゴンの描画処理
void HitPlayer(int nDamage);//ヒット処理
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer(void);//プレイヤーバッファの取得
Player* GetPlayer(void);

#endif _PLAYER_H_