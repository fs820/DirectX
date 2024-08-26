//---------------------------------------
//
//スコア処理の定義・宣言[score.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _SCORE_H_
#define _SCORE_H_

#include"main.h"

#define SCORE_MAX (10)
#define U_MAX_S (10)
#define V_MAX_S (1)
#define SCORE_WIDTH (350.0f)
#define SCORE_HEIGHT (70.0f)
#define DIGIT (10)//進数

#define SCORE_HIT (100)
#define SCORE_DES (1000)
#define SCORE_MISS (1000)
#define SCORE_DIE (10000)

//プロトタイプ宣言
void InitScore(void);//ポリゴンの初期化処理
void UninitScore(void);//ポリゴンの終了処理
void UpdateScore(void);//ポリゴンの更新処理
void DrawScore(void);//ポリゴンの描画処理
void SetScore(int nScore);//スコア書き換え
void AddScore(int nValue);//スコア加算
int GetScore(void);
int Digit(int nData, int nDigit);//分割処理

#endif _SCORE_H_
