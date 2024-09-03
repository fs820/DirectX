//---------------------------------------
//
//プレイヤー2表示処理の定義・宣言[player2.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _PLAYER2_H_
#define _PLAYER2_H_

#include"main.h"
#include"player.h"

//プレイヤー情報
#define PLAYER2_WIDTH (160)//幅
#define PLAYER2_HEIGHT (90)//高さ

#define U_MAX2 (1)//テクスチャの数(横)
#define V_MAX2 (1)//テクスチャの数(縦)

void InitPlayer2(void);//ポリゴンの初期化処理
void UninitPlayer2(void);//ポリゴンの終了処理
void UpdatePlayer2(void);//ポリゴンの更新処理
void DrawPlayer2(void);//ポリゴンの描画処理
void HitPlayer2(int nDamage);//ヒット処理
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer2(void);//プレイヤーバッファの取得
Player* GetPlayer2(void);

#endif _PLAYER2_H_