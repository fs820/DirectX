//---------------------------------------
//
//�f���v���C�R���s���[�^�[�\�������̒�`�E�錾[democpu.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _DEMOCPU_H_
#define _DEMOCPU_H_

#include"main.h"
#include"player.h"

//�v���C���[���
#define DEMOCPU_WIDTH (160)//��
#define DEMOCPU_HEIGHT (90)//����
#define DEMOCPU_SPEED_DEF (0.0f)
#define DEMOCPU_SPEED_X (3.0f)//����
#define DEMOCPU_SPEED_Y (3.0f)
#define DEMOCPU_INA (0.2f)
#define DEMOCPU_ROT_SPEED (0.05f)
#define DEMOCPU_SIZE_SPEED (3.0f)
#define DEMOCPU_SIZE_MAX (300.0f)
#define DEMOCPU_SIZE_MIN (5.0f)

#define U_MAX_C (1)//�e�N�X�`���̐�(��)
#define V_MAX_C (1)//�e�N�X�`���̐�(�c)

typedef struct
{
	int nCounterAnim;//�v���C���[�̍X�V�Ԋu
	int nPatternAnim;//�v���C���[�̃A�j���[�V�����p�^�[���Ǘ�
	D3DXVECTOR3 pos;//�v���C���[���W�Ǘ�
	D3DXVECTOR3 move;//�v���C���[�X�s�[�h�Ǘ�
	D3DXVECTOR3 rot;//�v���C���[�����Ǘ�
	D3DXVECTOR3 rotDef;//�v���C���[�����Ǘ�
	float fLength;//�Ίp���̒���(�傫��)
	float fLengthDef;//�Ίp���̒���(�傫��)�̏����l
	float fAngle;//�Ίp���̊p�x
	int nLife;//�̗�
	PLAYERSTATE state;//���
	int nCounterState;
}DEMOCPU;

void InitDemoCpu(void);//�|���S���̏���������
void UninitDemoCpu(void);//�|���S���̏I������
void UpdateDemoCpu(void);//�|���S���̍X�V����
void DrawDemoCpu(void);//�|���S���̕`�揈��
void HitDemoCpu(int nDamage);//�q�b�g����
PDIRECT3DVERTEXBUFFER9 GetDemoCpuBuffer(void);//�v���C���[�o�b�t�@�̎擾
DEMOCPU* GetDemoCpu(void);

#endif _DEMOCPU_H_