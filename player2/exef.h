//---------------------------------------
//
//’e•\¦ˆ—‚Ì’è‹`EéŒ¾[exef.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EXEF_H_
#define _EXEF_H_

#include"main.h"

#define MAX_EXEF (256)//Å‘å”
#define EF_NUM (20)
#define EXEF_LIFE (120)

void InitExef(void);//’e‚Ì‰Šú‰»ˆ—
void UninitExef(void);//’e‚ÌI—¹ˆ—
void UpdateExef(void);//’e‚ÌXVˆ—
void DrawExef(void);//’e‚Ì•`‰æˆ—
void SetExef(D3DXVECTOR3 pos,float fLength);//’Êí’e

#endif _EXEF_H_