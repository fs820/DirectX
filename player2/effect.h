//---------------------------------------
//
//�e�\�������̒�`�E�錾[bullet.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"

#define MAX_EFFECT (16384)//�ő吔
#define EFFECT_LIFE (100)

void InitEffect(void);//�e�̏���������
void UninitEffect(void);//�e�̏I������
void UpdateEffect(void);//�e�̍X�V����
void DrawEffect(void);//�e�̕`�揈��
void SetEffect(D3DXVECTOR3 pos,D3DXVECTOR3 move,D3DXCOLOR col, float fRadius, int nLife);//�ʏ�e

#endif _EFFECT_H_
