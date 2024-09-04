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
#include"effect.h"
#include"exef.h"

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
	InitEffect();
	InitExef();
	InitExplosion();//爆発
	InitScore();//スコア
	SetScore(0,true);//スコア書き換え
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
	SetScore(0,false);

	UninitScore();//スコア
	UninitExplosion();//爆発
	UninitExef();
	UninitEffect();
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
	UpdateEffect();
	UpdateExef();
	UpdateExplosion();//爆発
	UpdateScore();//スコア

	switch (g_gameStateDemo)
	{
	case GAMESTATE_NORMAL:
		if (pDemoCpu->state == PLAYERSTATE_DIE)
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
	else if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_START,CONTROLLER_MAX) == true||GetMouseTrigger(MOUSE_LEFT)==true)
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

	for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
	{
		if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
		{
			if (GetdJoykeyTrigger(ELEKEY_START, (CONTROLLER)i))
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
		else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
		{
			if (GetdJoykeyTrigger(PSKEY_START, (CONTROLLER)i))
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
		else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
		{
			if (GetdJoykeyTrigger(NINKEY_＋, (CONTROLLER)i))
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
		else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
		{
			if (GetdJoykeyTrigger(DKEY_START, (CONTROLLER)i))
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
	DrawEffect();
	DrawExef();
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