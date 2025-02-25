//---------------------------------------
//
//弾表示処理の定義・宣言[bullet.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"

#define MAX_EFFECT (16384)//最大数
#define EFFECT_LIFE (100)

void InitEffect(void);//弾の初期化処理
void UninitEffect(void);//弾の終了処理
void UpdateEffect(void);//弾の更新処理
void DrawEffect(void);//弾の描画処理
void SetEffect(D3DXVECTOR3 pos,D3DXVECTOR3 move,D3DXCOLOR col, float fRadius, int nLife);//通常弾

#endif _EFFECT_H_
