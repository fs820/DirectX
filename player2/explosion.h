//---------------------------------------
//
//�e�\�������̒�`�E�錾[explosion.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#define U_MAX_E (8)//�e�N�X�`���̐�(��)
#define V_MAX_E (8)//�e�N�X�`���̐�(�c)

#include"main.h"

//�v���g�^�C�v�錾
void InitExplosion(void);//�e�̏���������
void UninitExplosion(void);//�e�̏I������
void UpdateExplosion(void);//�e�̍X�V����
void DrawExplosion(void);//�e�̕`�揈��
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, float fLengthBullet);//�ʏ�e

#endif _EXPLOSION_H_
