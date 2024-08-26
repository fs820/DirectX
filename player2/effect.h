//---------------------------------------
//
//’e•\¦ˆ—‚Ì’è‹`EéŒ¾[bullet.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"

#define MAX_EFFECT (4096)//Å‘å”
#define EFFECT_LIFE (100)

void InitEffect(void);//’e‚Ì‰Šú‰»ˆ—
void UninitEffect(void);//’e‚ÌI—¹ˆ—
void UpdateEffect(void);//’e‚ÌXVˆ—
void DrawEffect(void);//’e‚Ì•`‰æˆ—
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXCOLOR col, float fRadius, int nLife,int Bullet);//’Êí’e

#endif _EFFECT_H_
