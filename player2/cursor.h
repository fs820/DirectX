//---------------------------------------
//
//�J�[�\�������̒�`�E�錾[cursor.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _CURSOR_H_
#define _CURSOR_H_

#include"main.h"

//�v���C���[���
#define CURSOR_WIDTH (100)//��
#define CURSOR_HEIGHT (100)//����
#define CURSOR_SPEED_DEF (0.0f)
#define CURSOR_SPEED_X (20.0f)//����
#define CURSOR_SPEED_Y (20.0f)

#define U_MAX_CU (18)//�e�N�X�`���̐�(��)
#define V_MAX_CU (11)//�e�N�X�`���̐�(�c)

typedef enum
{
	CURSORSTATE_NONE = 0,
	CURSORSTATE_NORMAL,
	CURSORSTATE_POINT,
	CURSORSTATE_MAX
}CURSORSTATE;

typedef struct
{
	int nCounterAnim;//�v���C���[�̍X�V�Ԋu
	int nPatternAnim;//�v���C���[�̃A�j���[�V�����p�^�[���Ǘ�
	D3DXVECTOR3 pos;//�v���C���[���W�Ǘ�
	D3DXVECTOR3 move;//�v���C���[�X�s�[�h�Ǘ�
	CURSORSTATE state;//���
	bool bUse;
}CURSOR;

void InitCursor(void);//�|���S���̏���������
void UninitCursor(void);//�|���S���̏I������
void UpdateCursor(void);//�|���S���̍X�V����
void DrawCursor(void);//�|���S���̕`�揈��
PDIRECT3DVERTEXBUFFER9 GetCursorBuffer(void);//�v���C���[�o�b�t�@�̎擾
CURSOR* GetCursorIn(void);
void CrCursor(bool bUse);

#endif _CURSOR_H_
