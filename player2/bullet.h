//---------------------------------------
//
//e\¦Ìè`Eé¾[bullet.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BULLET_H_
#define _BULLET_H_

#include"main.h"

#define MAX_BULLET (256)//Åå
#define MAX_BULLET_TYPE (4)
#define BULLET_WIDTH (25.0f)
#define BULLET_HEIGHT (25.0f)
#define BULLET_SPEED (15.0f)
#define BULLET_INTER (5)
#define ALL_BULLET (10)
#define ALL_BULLETS (6)
#define BULLET_LIFE (100)

//íÞ
typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_PLAYER2,
	BULLETTYPE_PLAYER3,
	BULLETTYPE_PLAYER4,
	BULLETTYPE_ENEMY,
	BULLETTYPE_BOSS,
	BULLETTYPE_MAX
}BULLETTYPE;

void InitBullet(void);//eÌú»
void UninitBullet(void);//eÌI¹
void UpdateBullet(void);//eÌXV
void DrawBullet(void);//eÌ`æ
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fLengthPlayer, int nLife, BULLETTYPE type,int nInter);//Êíe
void Set3Bullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//3weye
void SetAllBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//SûÊe
void SetSearchBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter);//U±e

#endif _BULLET_H_
