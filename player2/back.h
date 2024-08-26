//---------------------------------------
//
//背景表示処理の定義・宣言[back.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BACK_H_
#define _BACK_H_

#include"main.h"

#define BACK_MAX (3)

void InitBack(void);//ポリゴンの初期化処理
void UninitBack(void);//ポリゴンの終了処理
void UpdateBack(void);//ポリゴンの更新処理
void DrawBack(void);//ポリゴンの描画処理

#endif _BACK_H_