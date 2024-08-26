//---------------------------------------
//
//弾表示処理の定義・宣言[explosion.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#define U_MAX_E (8)//テクスチャの数(横)
#define V_MAX_E (8)//テクスチャの数(縦)

#include"main.h"

//プロトタイプ宣言
void InitExplosion(void);//弾の初期化処理
void UninitExplosion(void);//弾の終了処理
void UpdateExplosion(void);//弾の更新処理
void DrawExplosion(void);//弾の描画処理
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, float fLengthBullet);//通常弾

#endif _EXPLOSION_H_
