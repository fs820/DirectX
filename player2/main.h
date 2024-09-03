//------------------------------------------
//
//�e�N�X�`���\���v���O�����̃��C���̏����̒�`�E�錾[main.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _MAIN_H_
#define _MAIN_H_

#include<windows.h>
#include<time.h>
#include"d3dx9.h"
#include"dinput.h" // ���͂ɕK�v�ȃt�@�C��
#include"Xinput.h" // Xinput(�p�b�h����)
#include"xaudio2.h"

// ���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"uuid.lib")

// �}�N����`
#define DPS (1000/60) // �X�V�X�s�[�h
#define UP_TIME (3) // �e�N�X�`���X�s�[�h

// ���O
#define CLASS_NAME "WindowClass" // �N���X��
#define WINDOW_NAME "�u�E�҂͌��Ǖ��킪�������ƂɋC�t�����悤�ł��v�ɂȂ�\��̂���" // �E�C���h�E��
#define DATA_FILE "data\\DATA\\data.bin"
#define TEXTURE_CURSOR "data\\TEXTURE\\Cursor.png"
#define TEXTURE_TITLE "data\\TEXTURE\\title000.png"
#define TEXTURE_SELECT1 "data\\TEXTURE\\Enter or Start.png"
#define TEXTURE_SELECT2 "data\\TEXTURE\\Select.png"
#define TEXTURE_RESULT "data\\TEXTURE\\result000.png"
#define TEXTURE_SELECTRESTART "data\\TEXTURE\\Restart.png"
#define TEXTURE_NAME2 "data\\TEXTURE\\gita.png" // �e�N�X�`����2
#define TEXTURE_NAME_BULLET "data\\TEXTURE\\bullet000.png" // �e�N�X�`����2
#define TEXTURE_NAME_BG1 "data\\TEXTURE\\bg100.png"
#define TEXTURE_NAME_BG2 "data\\TEXTURE\\bg101.png"
#define TEXTURE_NAME_BG3 "data\\TEXTURE\\bg102.png"
#define TEXTURE_NAME_EXP "data\\TEXTURE\\explosion001.png"
#define TEXTURE_NAME_ENE "data\\TEXTURE\\gi.jpg"
#define TEXTURE_NAME_ENE2 "data\\TEXTURE\\anko.png"
#define TEXTURE_NAME_ENE3 "data\\TEXTURE\\anko2.png"
#define TEXTURE_NAME_ENE4 "data\\TEXTURE\\cat.png"
#define TEXTURE_SCORE "data\\TEXTURE\\number000.png"
#define TEXTURE_SCORE2 "data\\TEXTURE\\number001.png"
#define TEXTURE_SELECTBACK "data\\TEXTURE\\Back to Title.png"
#define TEXTURE_SELECTRETRY "data\\TEXTURE\\Retry.png"
#define TEXTURE_SELECTEXIT "data\\TEXTURE\\Exit.png"
#define TEXTURE_EFFECT "data\\TEXTURE\\effect000.jpg"

// �傫��
#define SCREEN_WIDTH (1280) // ��
#define SCREEN_HEIGHT (720) // ����

// ���_
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // ���_�t�H�[�}�b�g
#define VT_MAX (4)
#define UV_DEF (1.0f) // �e�N�X�`���̃f�t�H���g�̃T�C�Y

// �R���g���[���[
#define	ELE_CON "GP30X-Dinput"
#define PS_CON "Wireless Controller"
#define NIN_CON "Pro Controller"

// ���_���̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos; // ���W
	float rhw; // ���W�ϊ��W��(1.0f)
	D3DCOLOR col; // �J���[
	D3DXVECTOR2 tex; // �e�N�X�`�����W
} VERTEX_2D;

// ��ʂ̎��
typedef enum
{
	MODE_TITLE = 0,
	MODE_DEMO,
	MODE_TUTO,
	MODE_GAME,
	MODE_VS,
	MODE_RESULT,
	MODE_RANK,
	MODE_OPTION,
	MODE_MAX
} MODE;

// �v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // �E�C���h�E�v���V�[�W��
HRESULT Init(HINSTANCE hInstanse, HWND hWnd, BOOL bWindow = TRUE); // ����������
void Uninit(void); // �I������
void Update(void); // �X�V����
void Draw(void); // �`�揈��
LPDIRECT3DDEVICE9 GetDevice(void); // �f�o�C�X�̎擾
void SetMode(MODE mode);
MODE GetMode(void);
HWND GethWnd(void);
HINSTANCE GethInstanse(void);

void ConfineCursorToWindow(HWND hwnd);
void ReleaseCursor(void);

void ToggleFullScreen(HWND hWnd);
void ResetDevice(void);

#endif // _MAIN_H_