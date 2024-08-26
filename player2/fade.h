//------------------------------------------
//
//�t�F�[�h�����̒�`�E�錾[fade.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _FADE_H_
#define _FADE_H_

#include"main.h"

//�t�F�[�h�̏��
typedef enum
{
	FADE_NONE=0,
	FADE_IN,
	FADE_OUT,
	FADE_MAX
}FADE;

void InitFade(MODE modeNext);//����������
void UninitFade(void);//�I������
void UpdateFade(void);//�X�V����
void DrawFade(void);//�`�揈��
void SetFade(MODE modeNext);
FADE GetFade(void);

#endif  _FADE_H_

