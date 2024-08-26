//------------------------------------------
//
//デモ画面の処理[demo.cpp]
//Author fuma sato
//
//------------------------------------------

#include"demo.h"
#include"democpu.h"
#include"back.h"
#include"bullet.h"
#include"explosion.h"
#include"enemy.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
#include"score.h"

#define DEMOEND_TIME (1200)

//グローバル変数
GAMESTATE g_gameStateDemo = GAMESTATE_NONE;
int g_nCounterGameStateDemo = 0;

//--------------------
//初期化処理
//--------------------
void InitDemo(void)
{
	InitBack();//背景
	InitBullet();//弾
	InitDemoCpu();//プレイヤー
	InitEnemy();//敵
	InitExplosion();//爆発
	InitScore();//スコア
	SetScore(0);//スコア書き換え
	SetEnemy(D3DXVECTOR3(1000.0f, 200.0f, 0.0f), 0);//敵発生
	SetEnemy(D3DXVECTOR3(1000.0f, 500.0f, 0.0f), 1);//敵発生
	SetEnemy(D3DXVECTOR3(1000.0f, 350.0f, 0.0f), 2);//敵発生
	SetEnemy(D3DXVECTOR3(700.0f, 350.0f, 0.0f), 3);//敵発生

	PlaySound(SOUND_LABEL_BGM2);
}

//--------------------
//終了処理
//--------------------
void UninitDemo(void)
{
	//サウンド
	StopSound();

	UninitScore();//スコア
	UninitExplosion();//爆発
	UninitEnemy();//敵
	UninitDemoCpu();//プレイヤー
	UninitBullet();//弾
	UninitBack();//背景
}

//--------------------
//更新処理
//--------------------
void UpdateDemo(void)
{
	int NumEnemy;
	DEMOCPU* pDemoCpu;
	static int DemoEndCnt = 0;
	DemoEndCnt++;

	pDemoCpu = GetDemoCpu();
	NumEnemy = GetNumEnemy();

	UpdateBack();//背景
	UpdateBullet();//弾
	UpdateDemoCpu();//プレイヤー
	UpdateEnemy();//敵
	UpdateExplosion();//爆発
	UpdateScore();//スコア

	switch (g_gameStateDemo)
	{
	case GAMESTATE_NORMAL:
		if (pDemoCpu->state == DEMOCPUSTATE_DIE)
		{
			SetGameStateDemo(GAMESTATE_END);
		}
		else if (NumEnemy <= 0)
		{
			SetGameStateDemo(GAMESTATE_END);
		}
		break;
	case GAMESTATE_END:
		g_nCounterGameStateDemo++;
		if (g_nCounterGameStateDemo >= INTER)
		{
			DemoEndCnt = 0;
			FADE fade;
			fade = GetFade();
			if (fade == FADE_NONE)
			{
				//サウンド
				StopSound();
				//切替
				SetFade(MODE_TITLE);
			}
		}
		break;
	}

	if (DemoEndCnt>=DEMOEND_TIME)
	{
		DemoEndCnt = 0;
		FADE fade;
		fade = GetFade();
		if (fade == FADE_NONE)
		{
			//サウンド
			StopSound();
			//切替
			SetFade(MODE_TITLE);
		}
	}
	else if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_START) == true||GetMouseTrigger(MOUSE_LEFT)==true||GetdJoykeyTrigger(11)==true)
	{
		DemoEndCnt = 0;
		FADE fade;
		fade = GetFade();
		if (fade == FADE_NONE)
		{
			//サウンド
			StopSound();
			//切替
			SetFade(MODE_TITLE);
		}
	}
}

//--------------------
//描画処理
//--------------------
void DrawDemo(void)
{
	DrawBack();//背景
	DrawBullet();//弾
	DrawDemoCpu();//プレイヤー
	DrawEnemy();//敵
	DrawExplosion();//爆発
	DrawScore();//スコア
}

//----------------------
//ゲーム状態切替
//----------------------
void SetGameStateDemo(GAMESTATE state)
{
	g_gameStateDemo = state;
	g_nCounterGameStateDemo = 0;
}

//-----------------------
//ゲーム状態取得
//-----------------------
GAMESTATE GetGameStateDemo(void)
{
	return g_gameStateDemo;
}