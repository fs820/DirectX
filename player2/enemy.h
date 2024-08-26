//---------------------------------------
//
//敵表示処理の定義・宣言[enemy.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _ENEMY_H_

#define _ENEMY_H_

#include"main.h"

//マクロ定義
#define MAX_ENEMY (128)
#define ENEMY_WIDTH (200)//幅
#define ENEMY_HEIGHT (200)//高さ
#define ENEMY_WIDTH2 (193)//幅
#define ENEMY_HEIGHT2 (109.5f)//高さ
#define ENEMY_WIDTH3 (246.5f)//幅
#define ENEMY_HEIGHT3 (184.5f)//高さ
#define ENEMY_WIDTH4 (111)//幅
#define ENEMY_HEIGHT4 (139)//高さ

typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX,
}ENEMYSTATE;

typedef struct
{
	D3DXVECTOR3 pos;
	int nType;
	ENEMYSTATE state;
	int nCounterState;
	float fLength;
	int nLife;
	bool bUse;
}Enemy;

void InitEnemy(void);//ポリゴンの初期化処理
void UninitEnemy(void);//ポリゴンの終了処理
void UpdateEnemy(void);//ポリゴンの更新処理
void DrawEnemy(void);//ポリゴンの描画処理
void SetEnemy(D3DXVECTOR3 pos, int nType);
void HitEnemy(int nCntEnemy, int nDamage);
Enemy* GetEnemy(void);
int GetNumEnemy(void);

#endif _ENEMY_H_

