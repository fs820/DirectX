//---------------------------------------
//
//�v���C���[2�\�������̒�`�E�錾[player2.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _PLAYER2_H_
#define _PLAYER2_H_

#include"main.h"
#include"player.h"

//�v���C���[���
#define PLAYER2_WIDTH (160)//��
#define PLAYER2_HEIGHT (90)//����

#define U_MAX2 (1)//�e�N�X�`���̐�(��)
#define V_MAX2 (1)//�e�N�X�`���̐�(�c)

void InitPlayer2(void);//�|���S���̏���������
void UninitPlayer2(void);//�|���S���̏I������
void UpdatePlayer2(void);//�|���S���̍X�V����
void DrawPlayer2(void);//�|���S���̕`�揈��
void HitPlayer2(int nDamage);//�q�b�g����
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer2(void);//�v���C���[�o�b�t�@�̎擾
Player* GetPlayer2(void);

#endif _PLAYER2_H_