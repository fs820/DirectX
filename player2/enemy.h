//---------------------------------------
//
//�G�\�������̒�`�E�錾[enemy.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _ENEMY_H_

#define _ENEMY_H_

#include"main.h"

//�}�N����`
#define MAX_ENEMY (128)
#define ENEMY_WIDTH (200)//��
#define ENEMY_HEIGHT (200)//����
#define ENEMY_WIDTH2 (193)//��
#define ENEMY_HEIGHT2 (109.5f)//����
#define ENEMY_WIDTH3 (246.5f)//��
#define ENEMY_HEIGHT3 (184.5f)//����
#define ENEMY_WIDTH4 (111)//��
#define ENEMY_HEIGHT4 (139)//����

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

void InitEnemy(void);//�|���S���̏���������
void UninitEnemy(void);//�|���S���̏I������
void UpdateEnemy(void);//�|���S���̍X�V����
void DrawEnemy(void);//�|���S���̕`�揈��
void SetEnemy(D3DXVECTOR3 pos, int nType);
void HitEnemy(int nCntEnemy, int nDamage);
Enemy* GetEnemy(void);
int GetNumEnemy(void);

#endif _ENEMY_H_

