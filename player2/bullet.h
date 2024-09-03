//---------------------------------------
//
//�e�\�������̒�`�E�錾[bullet.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BULLET_H_
#define _BULLET_H_

#include"main.h"

#define MAX_BULLET (256)//�ő吔
#define MAX_BULLET_TYPE (4)
#define BULLET_WIDTH (25.0f)
#define BULLET_HEIGHT (25.0f)
#define BULLET_SPEED (15.0f)
#define BULLET_INTER (5)
#define ALL_BULLET (10)
#define ALL_BULLETS (6)
#define BULLET_LIFE (100)

//���
typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_ENEMY,
	BULLETTYPE_BOSS,
	BULLETTYPE_MAX
}BULLETTYPE;

void InitBullet(void);//�e�̏���������
void UninitBullet(void);//�e�̏I������
void UpdateBullet(void);//�e�̍X�V����
void DrawBullet(void);//�e�̕`�揈��
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fLengthPlayer, int nLife, BULLETTYPE type,int nInter);//�ʏ�e
void Set3Bullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//3wey�e
void SetAllBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//�S���ʒe
void SetSearchBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//�U���e

#endif _BULLET_H_
