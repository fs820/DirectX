//---------------------------------------
//
//�e�\�������̒�`�E�錾[exef.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EXEF_H_
#define _EXEF_H_

#include"main.h"

#define MAX_EXEF (256)//�ő吔
#define EF_NUM (20)
#define EXEF_LIFE (120)

void InitExef(void);//�e�̏���������
void UninitExef(void);//�e�̏I������
void UpdateExef(void);//�e�̍X�V����
void DrawExef(void);//�e�̕`�揈��
void SetExef(D3DXVECTOR3 pos,float fLength);//�ʏ�e

#endif _EXEF_H_