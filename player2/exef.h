//---------------------------------------
//
//弾表示処理の定義・宣言[exef.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EXEF_H_
#define _EXEF_H_

#include"main.h"

#define MAX_EXEF (256)//最大数
#define EF_NUM (20)
#define EXEF_LIFE (120)

void InitExef(void);//弾の初期化処理
void UninitExef(void);//弾の終了処理
void UpdateExef(void);//弾の更新処理
void DrawExef(void);//弾の描画処理
void SetExef(D3DXVECTOR3 pos,float fLength);//通常弾

#endif _EXEF_H_