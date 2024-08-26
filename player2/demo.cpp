//------------------------------------------
//
//�f����ʂ̏���[demo.cpp]
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

//�O���[�o���ϐ�
GAMESTATE g_gameStateDemo = GAMESTATE_NONE;
int g_nCounterGameStateDemo = 0;

//--------------------
//����������
//--------------------
void InitDemo(void)
{
	InitBack();//�w�i
	InitBullet();//�e
	InitDemoCpu();//�v���C���[
	InitEnemy();//�G
	InitExplosion();//����
	InitScore();//�X�R�A
	SetScore(0);//�X�R�A��������
	SetEnemy(D3DXVECTOR3(1000.0f, 200.0f, 0.0f), 0);//�G����
	SetEnemy(D3DXVECTOR3(1000.0f, 500.0f, 0.0f), 1);//�G����
	SetEnemy(D3DXVECTOR3(1000.0f, 350.0f, 0.0f), 2);//�G����
	SetEnemy(D3DXVECTOR3(700.0f, 350.0f, 0.0f), 3);//�G����

	PlaySound(SOUND_LABEL_BGM2);
}

//--------------------
//�I������
//--------------------
void UninitDemo(void)
{
	//�T�E���h
	StopSound();

	UninitScore();//�X�R�A
	UninitExplosion();//����
	UninitEnemy();//�G
	UninitDemoCpu();//�v���C���[
	UninitBullet();//�e
	UninitBack();//�w�i
}

//--------------------
//�X�V����
//--------------------
void UpdateDemo(void)
{
	int NumEnemy;
	DEMOCPU* pDemoCpu;
	static int DemoEndCnt = 0;
	DemoEndCnt++;

	pDemoCpu = GetDemoCpu();
	NumEnemy = GetNumEnemy();

	UpdateBack();//�w�i
	UpdateBullet();//�e
	UpdateDemoCpu();//�v���C���[
	UpdateEnemy();//�G
	UpdateExplosion();//����
	UpdateScore();//�X�R�A

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
				//�T�E���h
				StopSound();
				//�ؑ�
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
			//�T�E���h
			StopSound();
			//�ؑ�
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
			//�T�E���h
			StopSound();
			//�ؑ�
			SetFade(MODE_TITLE);
		}
	}
}

//--------------------
//�`�揈��
//--------------------
void DrawDemo(void)
{
	DrawBack();//�w�i
	DrawBullet();//�e
	DrawDemoCpu();//�v���C���[
	DrawEnemy();//�G
	DrawExplosion();//����
	DrawScore();//�X�R�A
}

//----------------------
//�Q�[����Ԑؑ�
//----------------------
void SetGameStateDemo(GAMESTATE state)
{
	g_gameStateDemo = state;
	g_nCounterGameStateDemo = 0;
}

//-----------------------
//�Q�[����Ԏ擾
//-----------------------
GAMESTATE GetGameStateDemo(void)
{
	return g_gameStateDemo;
}