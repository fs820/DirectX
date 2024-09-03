//---------------------------------------
//
//�v���C���[�\�������̒�`�E�錾[player.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include"main.h"

//�v���C���[���
#define PLAYER_WIDTH (160)//��
#define PLAYER_HEIGHT (90)//����
#define PLAYER_SPEED_DEF (0.0f)
#define PLAYER_SPEED_X (3.0f)//����
#define PLAYER_SPEED_Y (3.0f)
#define PLAYER_INA (0.2f)
#define PLAYER_ROT_SPEED (0.05f)
#define PLAYER_SIZE_SPEED (3.0f)
#define PLAYER_SIZE_MAX (300.0f)
#define PLAYER_SIZE_MIN (5.0f)

#define U_MAX (1)//�e�N�X�`���̐�(��)
#define V_MAX (1)//�e�N�X�`���̐�(�c)

typedef enum
{
	PLAYERSTATE_APPEAR = 0,
	PLAYERSTATE_NORMAL,
	PLAYERSTATE_DAMAGE,
	PLAYERSTATE_DIE,
	PLAYERSTATE_MAX
}PLAYERSTATE;

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
	int nRema;
	int nLife;//�̗�
	PLAYERSTATE state;//���
	int nCounterState;
	bool bDisp;
}Player;

void InitPlayer(void);//�|���S���̏���������
void UninitPlayer(void);//�|���S���̏I������
void UpdatePlayer(void);//�|���S���̍X�V����
void DrawPlayer(void);//�|���S���̕`�揈��
void HitPlayer(int nDamage);//�q�b�g����
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer(void);//�v���C���[�o�b�t�@�̎擾
Player* GetPlayer(void);

#endif _PLAYER_H_