//---------------------------------------
//
//�X�R�A�����̒�`�E�錾[score.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _SCORE_H_
#define _SCORE_H_

#include"main.h"

#define SCORE_MAX (10)
#define U_MAX_S (10)
#define V_MAX_S (1)
#define SCORE_WIDTH (350.0f)
#define SCORE_HEIGHT (70.0f)
#define DIGIT (10)//�i��

#define SCORE_HIT (100)
#define SCORE_DES (1000)
#define SCORE_MISS (1000)
#define SCORE_DIE (10000)

//�v���g�^�C�v�錾
void InitScore(void);//�|���S���̏���������
void UninitScore(void);//�|���S���̏I������
void UpdateScore(void);//�|���S���̍X�V����
void DrawScore(void);//�|���S���̕`�揈��
void SetScore(int nScore);//�X�R�A��������
void AddScore(int nValue);//�X�R�A���Z
int GetScore(void);
int Digit(int nData, int nDigit);//��������

#endif _SCORE_H_
