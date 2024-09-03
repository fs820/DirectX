//---------------------------------------
//
//�{�X�\�������̒�`�E�錾[boss.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BOSS_H_

#define _BOSS_H_

#include"main.h"

//�}�N����`
#define MAX_BOSS (8)
#define NUM_BOSS (4)
#define BOSS_WIDTH (200)//��
#define BOSS_HEIGHT (200)//����
#define BOSS_WIDTH2 (193)//��
#define BOSS_HEIGHT2 (109.5f)//����
#define BOSS_WIDTH3 (246.5f)//��
#define BOSS_HEIGHT3 (184.5f)//����
#define BOSS_WIDTH4 (111)//��
#define BOSS_HEIGHT4 (139)//����

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

void InitBoss(void);//�|���S���̏���������
void UninitBoss(void);//�|���S���̏I������
void UpdateBoss(void);//�|���S���̍X�V����
void DrawBoss(void);//�|���S���̕`�揈��
void SetBoss(D3DXVECTOR3 pos, int nType);
void HitBoss(int nCntEnemy, int nDamage);
Boss* GetBoss(void);
int GetNumBoss(void);

#endif _BOSS_H_

