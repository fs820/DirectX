//---------------------------------------
//
//ボス表示処理の定義・宣言[boss.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BOSS_H_

#define _BOSS_H_

#include"main.h"

//マクロ定義
#define MAX_BOSS (8)
#define NUM_BOSS (4)
#define BOSS_WIDTH (200)//幅
#define BOSS_HEIGHT (200)//高さ
#define BOSS_WIDTH2 (193)//幅
#define BOSS_HEIGHT2 (109.5f)//高さ
#define BOSS_WIDTH3 (246.5f)//幅
#define BOSS_HEIGHT3 (184.5f)//高さ
#define BOSS_WIDTH4 (111)//幅
#define BOSS_HEIGHT4 (139)//高さ

typedef enum
{
	BOSSSTATE_NORMAL = 0,
	BOSSSTATE_DAMAGE,
	BOSSSTATE_MAX,
}BOSSSTATE;

typedef struct
{
	D3DXVECTOR3 pos;
	int nType;
	BOSSSTATE state;
	int nCounterState;
	float fLength;
	int nLife;
	bool bUse;
}Boss;

void InitBoss(void);//ポリゴンの初期化処理
void UninitBoss(void);//ポリゴンの終了処理
void UpdateBoss(void);//ポリゴンの更新処理
void DrawBoss(void);//ポリゴンの描画処理
void SetBoss(D3DXVECTOR3 pos, int nType);
void HitBoss(int nCntEnemy, int nDamage);
Boss* GetBoss(void);
int GetNumBoss(void);

#endif _BOSS_H_

