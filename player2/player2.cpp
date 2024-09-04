//----------------------------------------
//
//�v���C���[�\������[player2.cpp]
//Author fuma sato
//
//----------------------------------------

#include"player2.h"
#include"enemy.h"
#include"boss.h"
#include"input.h"
#include"back.h"
#include"bullet.h"
#include"explosion.h"
#include"exef.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTexturePlayer2;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer2 = NULL;//�o�b�t�@�̃|�C���^
Player g_Player2;//�v���C���[���
D3DXVECTOR3 g_moveBullet2;//�e���ˌ����E�X�s�[�h�Ǘ�

//----------------------
//�v���C���[�̏���������
//----------------------
void InitPlayer2(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer2,
		NULL
	);

	g_Player2.nCounterAnim = 0;
	g_Player2.nPatternAnim = 0;
	g_Player2.pos = D3DXVECTOR3(SCREEN_WIDTH-PLAYER_WIDTH, SCREEN_HEIGHT / 2, 0);//�����ʒu
	g_Player2.move = D3DXVECTOR3(PLAYER_SPEED_DEF, PLAYER_SPEED_DEF, PLAYER_SPEED_DEF);//�v���C���[�X�s�[�h
	g_Player2.rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI);//����
	g_Player2.rotDef = g_Player2.rot;//�����ۑ�
	g_Player2.fLength = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) / 2.0f;//�Ίp���̒���
	g_Player2.fLengthDef = g_Player2.fLength;//�����ۑ�
	g_Player2.fAngle = atan2f(PLAYER_WIDTH, PLAYER_HEIGHT);//�p�x
	g_Player2.nRema = 3;
	g_Player2.nLife = 100;
	g_Player2.state = PLAYERSTATE_NORMAL;
	g_Player2.nCounterState = 0;
	g_Player2.bDisp = true;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME2,
		&g_pTexturePlayer2
	);

	g_pVtxBuffPlayer2->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

	//���W�ݒ�
	pVtx[0].pos.x = g_Player2.pos.x + sinf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength;
	pVtx[0].pos.y = g_Player2.pos.y + cosf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player2.pos.x + sinf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength;
	pVtx[1].pos.y = g_Player2.pos.y + cosf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player2.pos.x + sinf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength;
	pVtx[2].pos.y = g_Player2.pos.y + cosf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player2.pos.x + sinf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength;
	pVtx[3].pos.y = g_Player2.pos.y + cosf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength;
	pVtx[3].pos.z = 0.0f;

	//rhw
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`��
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX);
	pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX, UV_DEF / V_MAX);

	g_pVtxBuffPlayer2->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
}

//-------------------
//�v���C���[�I������
//-------------------
void UninitPlayer2(void)
{
	SetVibrate(0.0f, CONTROLLER_1);
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer2 != NULL)
	{
		g_pTexturePlayer2->Release();
		g_pTexturePlayer2 = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer2 != NULL)
	{
		g_pVtxBuffPlayer2->Release();
		g_pVtxBuffPlayer2 = NULL;
	}
}

//-------------------
//�v���C���[�X�V����
//-------------------
void UpdatePlayer2(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^
	float* pStick;
	float* pMouseMove;
	float fMouseWheel;

	//�ړ�����

	//X
	if (IsXInputControllerConnected(CONTROLLER_2))
	{
		//���X�e�B�b�N����
		pStick = GetJoyStick(STICK_LEFT, CONTROLLER_2);
		g_Player2.move.x += *pStick * PLAYER_SPEED_X;
		g_Player2.move.y += -*(pStick + 1) * PLAYER_SPEED_Y;

		//�E�X�e�B�b�N����
		pStick = GetJoyStick(STICK_RIGHT, CONTROLLER_2);
		if (*pStick > STICK_DED || *(pStick + 1) > STICK_DED || *pStick < -STICK_DED || *(pStick + 1) < -STICK_DED)
		{
			g_Player2.rot.z = atan2f(*(pStick + 1), *pStick);
		}
	}
	//d
	else if (IsDirectInputControllerConnected(CONTROLLER_2))
	{

		//���X�e�B�b�N����
		pStick = GetdJoyStick(STICK_LEFT, CONTROLLER_2);
		g_Player2.move.x += *pStick * PLAYER_SPEED_X;
		g_Player2.move.y += *(pStick + 1) * PLAYER_SPEED_Y;

		//�E�X�e�B�b�N����
		pStick = GetdJoyStick(STICK_RIGHT, CONTROLLER_2);
		if (*pStick > STICK_DED || *(pStick + 1) > STICK_DED || *pStick < -STICK_DED || *(pStick + 1) < -STICK_DED)
		{
			g_Player2.rot.z = -atan2f(*(pStick + 1), *pStick);
		}
	}

	////�}�E�X����
	//pMouseMove = GetMouseMove();
	//g_Player2.move.x += *pMouseMove * PLAYER_SPEED_X;
	//g_Player2.move.y += *(pMouseMove + 1) * PLAYER_SPEED_Y;

	//fMouseWheel = GetMouseWheel();
	//g_Player2.rot.z += fMouseWheel;

	if (GetKeyboradPress(DIK_LEFT) == true || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_2) == true)//A�L�[
	{
		if (GetKeyboradPress(DIK_UP) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_2) == true)//W�L�[
		{//����
			g_Player2.move.x += sinf(D3DX_PI * -0.75f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * -0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetKeyboradPress(DIK_DOWN) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_2) == true)//S�L�[
		{//����
			g_Player2.move.x += sinf(D3DX_PI * -0.25f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * -0.25f) * PLAYER_SPEED_Y;
		}
		else
		{//��
			g_Player2.move.x += -PLAYER_SPEED_X;
		}
	}
	else if (GetKeyboradPress(DIK_RIGHT) == true || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_2) == true)//D�L�[
	{
		if (GetKeyboradPress(DIK_UP) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_2) == true)//W�L�[
		{//�E��
			g_Player2.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetKeyboradPress(DIK_DOWN) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_2) == true)//S�L�[
		{//�E��
			g_Player2.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED_Y;
		}
		else
		{//�E
			g_Player2.move.x += PLAYER_SPEED_X;
		}
	}
	else if (GetKeyboradPress(DIK_UP) == true || GetJoykeyPress(JOYKEY_UP, CONTROLLER_2) == true)//W�L�[
	{//��
		g_Player2.move.y += -PLAYER_SPEED_Y;
	}
	else if (GetKeyboradPress(DIK_DOWN) == true || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_2) == true)//S�L�[
	{//��
		g_Player2.move.y += PLAYER_SPEED_Y;
	}

	if (GetKeyboradPress(DIK_7) == true || GetJoykeyPress(JOYKEY_LB, CONTROLLER_2) == true)
	{
		g_Player2.rot.z += PLAYER_ROT_SPEED;
	}

	if (GetKeyboradPress(DIK_9) == true || GetJoykeyPress(JOYKEY_RB, CONTROLLER_2) == true)
	{
		g_Player2.rot.z += -PLAYER_ROT_SPEED;
	}

	if (GetKeyboradTrigger(DIK_8) == true || GetJoykeyTrigger(JOYKEY_L3, CONTROLLER_2) == true)
	{
		g_Player2.rot = g_Player2.rotDef;
	}

	if (GetKeyboradPress(DIK_1) == true || GetMousePress(MOUSE_B1) == true || GetJoykeyPress(JOYKEY_LT, CONTROLLER_2) == true)
	{
		if (g_Player2.fLength >= 0.0f && g_Player2.fLength <= PLAYER_SIZE_MAX)
		{
			g_Player2.fLength += PLAYER_SIZE_SPEED;
		}
	}

	if (GetKeyboradPress(DIK_3) == true || GetMousePress(MOUSE_B2) == true || GetJoykeyPress(JOYKEY_RT, CONTROLLER_2) == true)
	{
		if (g_Player2.fLength >= PLAYER_SIZE_MIN)
		{
			g_Player2.fLength += -PLAYER_SIZE_SPEED;
		}
	}

	if (GetKeyboradTrigger(DIK_2) == true || GetJoykeyTrigger(JOYKEY_R3, CONTROLLER_2) == true)
	{
		g_Player2.fLength = g_Player2.fLengthDef;
	}

	if (g_Player2.state != PLAYERSTATE_APPEAR && g_Player2.state != PLAYERSTATE_DIE)
	{
		if (GetKeyboradTrigger(DIK_RSHIFT) == true || GetMouseTrigger(MOUSE_LEFT) == true || GetJoykeyTrigger(JOYKEY_A, CONTROLLER_2) == true)
		{
			g_moveBullet2.x = sinf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
			g_moveBullet2.y = cosf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

			//�e�̐ݒ�
			SetBullet(g_Player2.pos, g_moveBullet2, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);
		}
		else if (GetKeyboradRelease(DIK_RCONTROL) == true || GetMouseRelease(MOUSE_RIGHT) == true || GetJoykeyRelease(JOYKEY_B, CONTROLLER_2) == true)
		{
			//�e�̐ݒ�
			Set3Bullet(g_Player2.pos, g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);
		}
		else if (GetKeyboradRepeat(DIK_0) == true || GetMouseRepeat(MOUSE_SENTER) == true || GetJoykeyRepeat(JOYKEY_X, CONTROLLER_2) == true)
		{
			//�e�̐ݒ�
			SetAllBullet(g_Player2.pos, g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);
		}
		else if (GetKeyboradRepeat(DIK_COMMA) == true || GetJoykeyRepeat(JOYKEY_Y, CONTROLLER_2) == true)
		{
			//�e�̐ݒ�
			SetSearchBullet(g_Player2.pos, g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);
		}
	}

	if (IsDirectInputControllerConnected(CONTROLLER_2))
	{
		if (GetdJoyPov(POV_UP, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.y += -PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_RIGHTUP, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.x += PLAYER_SPEED_X;
		}
		else if (GetdJoyPov(POV_RIGHTDOWN, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.y += PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_LEFTDOWN, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.x += sinf(D3DX_PI * -0.25f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * -0.25f) * PLAYER_SPEED_Y;
		}
		else if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.x += -PLAYER_SPEED_X;
		}
		else if (GetdJoyPov(POV_LEFTUP, 0, CONTROLLER_2) == true)
		{
			g_Player2.move.x += sinf(D3DX_PI * -0.75f) * PLAYER_SPEED_X;
			g_Player2.move.y += cosf(D3DX_PI * -0.75f) * PLAYER_SPEED_Y;
		}
	}

	if (!strcmp(ControllerName(CONTROLLER_2), ELE_CON))
	{
		if (GetdJoykeyPress(ELEKEY_LB, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(ELEKEY_RB, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(ELEKEY_L3, CONTROLLER_2) == true)
		{
			g_Player2.rot = g_Player2.rotDef;
		}

		if (GetdJoykeyPress(ELEKEY_LT, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= 0.0f && g_Player2.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player2.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(ELEKEY_RT, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player2.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(ELEKEY_R3, CONTROLLER_2) == true)
		{
			g_Player2.fLength = g_Player2.fLengthDef;
		}

		if (g_Player2.state != PLAYERSTATE_APPEAR && g_Player2.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(ELEKEY_A, CONTROLLER_2) == true)
			{
				g_moveBullet2.x = sinf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
				g_moveBullet2.y = cosf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

				//�e�̐ݒ�
				SetBullet(g_Player2.pos, g_moveBullet2, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);
			}
			else if (GetdJoykeyRelease(ELEKEY_B, CONTROLLER_2) == true)
			{
				//�e�̐ݒ�
				Set3Bullet(g_Player2.pos, g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);
			}
			else if (GetdJoykeyRepeat(ELEKEY_X, CONTROLLER_2) == true)
			{
				//�e�̐ݒ�
				SetAllBullet(g_Player2.pos, g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);
			}
			else if (GetdJoykeyRepeat(ELEKEY_Y, CONTROLLER_2) == true)
			{
				//�e�̐ݒ�
				SetSearchBullet(g_Player2.pos, g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);

			}
		}
	}
	else if (!strcmp(ControllerName(CONTROLLER_2), PS_CON))
	{
		if (GetdJoykeyPress(PSKEY_LB, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(PSKEY_RB, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(PSKEY_L3, CONTROLLER_2) == true)
		{
			g_Player2.rot = g_Player2.rotDef;
		}

		if (GetdJoykeyPress(PSKEY_LT, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= 0.0f && g_Player2.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player2.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(PSKEY_RT, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player2.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(PSKEY_R3, CONTROLLER_2) == true)
		{
			g_Player2.fLength = g_Player2.fLengthDef;
		}

		if (g_Player2.state != PLAYERSTATE_APPEAR && g_Player2.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(PSKEY_CI, CONTROLLER_2) == true)
			{
				g_moveBullet2.x = sinf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
				g_moveBullet2.y = cosf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

				//�e�̐ݒ�
				SetBullet(g_Player2.pos, g_moveBullet2, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);
			}
			else if (GetdJoykeyRelease(PSKEY_CR, CONTROLLER_2) == true)
			{
				//�e�̐ݒ�
				Set3Bullet(g_Player2.pos, g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);
			}
			else if (GetdJoykeyRepeat(PSKEY_TRA, CONTROLLER_2) == true)
			{
				//�e�̐ݒ�
				SetAllBullet(g_Player2.pos, g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);
			}
			else if (GetdJoykeyRepeat(PSKEY_SQ, CONTROLLER_2) == true)
			{
				//�e�̐ݒ�
				SetSearchBullet(g_Player2.pos, g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);
			}
		}
	}
	else if (!strcmp(ControllerName(CONTROLLER_2), NIN_CON))
	{
		if (GetdJoykeyPress(NINKEY_LB, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(NINKEY_RB, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(NINKEY_L3, CONTROLLER_2) == true)
		{
			g_Player2.rot = g_Player2.rotDef;
		}

		if (GetdJoykeyPress(NINKEY_LT, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= 0.0f && g_Player2.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player2.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(NINKEY_RT, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player2.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(NINKEY_R3, CONTROLLER_2) == true)
		{
			g_Player2.fLength = g_Player2.fLengthDef;
		}

		if (g_Player2.state != PLAYERSTATE_APPEAR && g_Player2.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(NINKEY_A, CONTROLLER_2) == true)
			{
				g_moveBullet2.x = sinf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
				g_moveBullet2.y = cosf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

				//�e�̐ݒ�
				SetBullet(g_Player2.pos, g_moveBullet2, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);
			}
			else if (GetdJoykeyRelease(NINKEY_B, CONTROLLER_2) == true)
			{
				//�e�̐ݒ�
				Set3Bullet(g_Player2.pos, g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);
			}
			else if (GetdJoykeyRepeat(NINKEY_X, CONTROLLER_2) == true)
			{
				//�e�̐ݒ�
				SetAllBullet(g_Player2.pos, g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);
			}
			else if (GetdJoykeyRepeat(NINKEY_Y, CONTROLLER_2) == true)
			{
				//�e�̐ݒ�
				SetSearchBullet(g_Player2.pos, g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);
			}
		}
	}
	else if (!IsXInputControllerConnected(CONTROLLER_2) && IsDirectInputControllerConnected(CONTROLLER_2))
	{
		if (GetdJoykeyPress(DKEY_LB, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyPress(DKEY_RB, CONTROLLER_2) == true)
		{
			g_Player2.rot.z += -PLAYER_ROT_SPEED;
		}

		if (GetdJoykeyTrigger(DKEY_L3, CONTROLLER_2) == true)
		{
			g_Player2.rot = g_Player2.rotDef;
		}

		if (GetdJoykeyPress(DKEY_LT, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= 0.0f && g_Player2.fLength <= PLAYER_SIZE_MAX)
			{
				g_Player2.fLength += PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyPress(DKEY_RT, CONTROLLER_2) == true)
		{
			if (g_Player2.fLength >= PLAYER_SIZE_MIN)
			{
				g_Player2.fLength += -PLAYER_SIZE_SPEED;
			}
		}

		if (GetdJoykeyTrigger(DKEY_R3, CONTROLLER_2) == true)
		{
			g_Player2.fLength = g_Player2.fLengthDef;
		}

		if (g_Player2.state != PLAYERSTATE_APPEAR && g_Player2.state != PLAYERSTATE_DIE)
		{
			if (GetdJoykeyTrigger(DKEY_A, CONTROLLER_2) == true)
			{
				g_moveBullet2.x = sinf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
				g_moveBullet2.y = cosf(g_Player2.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

				//�e�̐ݒ�
				SetBullet(g_Player2.pos, g_moveBullet2, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);
			}
			else if (GetdJoykeyRelease(DKEY_B, CONTROLLER_2) == true)
			{
				//�e�̐ݒ�
				Set3Bullet(g_Player2.pos, g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, 1);
			}
			else if (GetdJoykeyRepeat(DKEY_X, CONTROLLER_2) == true)
			{
				//�e�̐ݒ�
				SetAllBullet(g_Player2.pos, g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);
			}
			else if (GetdJoykeyRepeat(DKEY_Y, CONTROLLER_2) == true)
			{
				//�e�̐ݒ�
				SetSearchBullet(g_Player2.pos, g_Player2.rot.z, g_Player2.fLength, BULLET_LIFE, BULLETTYPE_PLAYER2, BULLET_INTER);
			}
		}
	}

	if (g_Player2.state != PLAYERSTATE_APPEAR && g_Player2.state != PLAYERSTATE_DIE)
	{
		Enemy* pEnemy;
		Boss* pBoss;
		Player* pPlayer;
		float EnemyWidth = 0.0f, EnemyHeight = 0.0f;

		pEnemy = GetEnemy();
		pBoss = GetBoss();
		pPlayer = GetPlayer();
		for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{//�G���g�p����Ă���
				switch (pEnemy->nType)
				{
				case 0:
					EnemyWidth = ENEMY_WIDTH;
					EnemyHeight = ENEMY_HEIGHT;
					break;
				case 1:
					EnemyWidth = ENEMY_WIDTH2;
					EnemyHeight = ENEMY_HEIGHT2;
					break;
				case 2:
					EnemyWidth = ENEMY_WIDTH3;
					EnemyHeight = ENEMY_HEIGHT3;
					break;
				case 3:
					EnemyWidth = ENEMY_WIDTH4;
					EnemyHeight = ENEMY_HEIGHT4;
					break;
				}
				if (pEnemy->pos.x - EnemyWidth / 2 <= g_Player2.pos.x + g_Player2.fLength && pEnemy->pos.x + EnemyWidth / 2 >= g_Player2.pos.x - g_Player2.fLength && pEnemy->pos.y - EnemyHeight / 2 <= g_Player2.pos.y + g_Player2.fLength && pEnemy->pos.y + EnemyHeight / 2 >= g_Player2.pos.y - g_Player2.fLength)
				{
					if (g_Player2.pos.x <= pEnemy->pos.x - EnemyWidth / 2)
					{
						g_Player2.pos.x = (pEnemy->pos.x - EnemyWidth / 2) - g_Player2.fLength;
					}
					else if (g_Player2.pos.x >= pEnemy->pos.x + EnemyWidth / 2)
					{
						g_Player2.pos.x = (pEnemy->pos.x + EnemyWidth / 2) + g_Player2.fLength;
					}
					else if (g_Player2.pos.y <= pEnemy->pos.y - EnemyHeight / 2)
					{
						g_Player2.pos.y = (pEnemy->pos.y - EnemyHeight / 2) - g_Player2.fLength;
					}
					else if (g_Player2.pos.y >= pEnemy->pos.y + EnemyHeight / 2)
					{
						g_Player2.pos.y = (pEnemy->pos.y + EnemyHeight / 2) + g_Player2.fLength;
					}
				}
			}
		}

		for (int i = 0; i < MAX_BOSS; i++)
		{
			if (pBoss->bUse == true)
			{//�G���g�p����Ă���
				switch (pBoss->nType)
				{
				case 0:
					EnemyWidth = BOSS_WIDTH;
					EnemyHeight = BOSS_HEIGHT;
					break;
				case 1:
					EnemyWidth = BOSS_WIDTH2;
					EnemyHeight = BOSS_HEIGHT2;
					break;
				case 2:
					EnemyWidth = BOSS_WIDTH3;
					EnemyHeight = BOSS_HEIGHT3;
					break;
				case 3:
					EnemyWidth = BOSS_WIDTH4;
					EnemyHeight = BOSS_HEIGHT4;
					break;
				}
				if (pBoss->pos.x - EnemyWidth / 2 <= g_Player2.pos.x + g_Player2.fLength && pBoss->pos.x + EnemyWidth / 2 >= g_Player2.pos.x - g_Player2.fLength && pBoss->pos.y - EnemyHeight / 2 <= g_Player2.pos.y + g_Player2.fLength && pBoss->pos.y + EnemyHeight / 2 >= g_Player2.pos.y - g_Player2.fLength)
				{
					if (g_Player2.pos.x <= pBoss->pos.x - EnemyWidth / 2)
					{
						g_Player2.pos.x = (pBoss->pos.x - EnemyWidth / 2) - g_Player2.fLength;
					}
					else if (g_Player2.pos.x >= pBoss->pos.x + EnemyWidth / 2)
					{
						g_Player2.pos.x = (pBoss->pos.x + EnemyWidth / 2) + g_Player2.fLength;
					}
					else if (g_Player2.pos.y <= pBoss->pos.y - EnemyHeight / 2)
					{
						g_Player2.pos.y = (pBoss->pos.y - EnemyHeight / 2) - g_Player2.fLength;
					}
					else if (g_Player2.pos.y >= pBoss->pos.y + EnemyHeight / 2)
					{
						g_Player2.pos.y = (pBoss->pos.y + EnemyHeight / 2) + g_Player2.fLength;
					}
				}
			}
		}


		if (pPlayer->pos.x - pPlayer->fLength <= g_Player2.pos.x + g_Player2.fLength && pPlayer->pos.x + pPlayer->fLength >= g_Player2.pos.x - g_Player2.fLength && pPlayer->pos.y - pPlayer->fLength <= g_Player2.pos.y + g_Player2.fLength && pPlayer->pos.y + pPlayer->fLength >= g_Player2.pos.y - g_Player2.fLength)
		{
			if (g_Player2.pos.x <= pPlayer->pos.x - pPlayer->fLength)
			{
				if (GetKeyboradPress(DIK_D) || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1) || JoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_1))
				{
					pPlayer->pos.x = (g_Player2.pos.x + g_Player2.fLength) + pPlayer->fLength;
				}
				else
				{
					g_Player2.pos.x = (pPlayer->pos.x - pPlayer->fLength) - g_Player2.fLength;
				}

				if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
				{
					if (GetdJoyPov(POV_RIGHT, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_RIGHT, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer->pos.x = (g_Player2.pos.x + g_Player2.fLength) + pPlayer->fLength;
					}
					else
					{
						g_Player2.pos.x = (pPlayer->pos.x - pPlayer->fLength) - g_Player2.fLength;
					}
				}
			}
			else if (g_Player2.pos.x >= pPlayer->pos.x + pPlayer->fLength)
			{
				if (GetKeyboradPress(DIK_A) || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1) || JoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_1))
				{
					pPlayer->pos.x = (g_Player2.pos.x - g_Player2.fLength) - pPlayer->fLength;
				}
				else
				{
					g_Player2.pos.x = (pPlayer->pos.x + pPlayer->fLength) + g_Player2.fLength;
				}

				if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
				{
					if (GetdJoyPov(POV_LEFT, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_LEFT, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer->pos.x = (g_Player2.pos.x - g_Player2.fLength) - pPlayer->fLength;
					}
					else
					{
						g_Player2.pos.x = (pPlayer->pos.x + pPlayer->fLength) + g_Player2.fLength;
					}
				}
			}
			else if (g_Player2.pos.y <= pPlayer->pos.y - pPlayer->fLength)
			{
				if (GetKeyboradPress(DIK_S) || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1) || JoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_1))
				{
					pPlayer->pos.y = (g_Player2.pos.y + g_Player2.fLength) + pPlayer->fLength;
				}
				else
				{
					g_Player2.pos.y = (pPlayer->pos.y - pPlayer->fLength) - g_Player2.fLength;
				}

				if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
				{
					if (GetdJoyPov(POV_DOWN, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer->pos.y = (g_Player2.pos.y + g_Player2.fLength) + pPlayer->fLength;
					}
					else
					{
						g_Player2.pos.y = (pPlayer->pos.y - pPlayer->fLength) - g_Player2.fLength;
					}
				}
			}
			else if (g_Player2.pos.y >= pPlayer->pos.y + pPlayer->fLength)
			{
				if (GetKeyboradPress(DIK_W) || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1) || JoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_1))
				{
					pPlayer->pos.y = (g_Player2.pos.y - g_Player2.fLength) - pPlayer->fLength;
				}
				else
				{
					g_Player2.pos.y = (pPlayer->pos.y + pPlayer->fLength) + g_Player2.fLength;
				}

				if (!IsXInputControllerConnected(CONTROLLER_1), IsDirectInputControllerConnected(CONTROLLER_1))
				{
					if (GetdJoyPov(POV_UP, 0, CONTROLLER_1) || dJoyStickPress(DIRESTICK_UP, STICK_LEFT, CONTROLLER_1))
					{
						pPlayer->pos.y = (g_Player2.pos.y - g_Player2.fLength) - pPlayer->fLength;
					}
					else
					{
						g_Player2.pos.y = (pPlayer->pos.y + pPlayer->fLength) + g_Player2.fLength;
					}
				}
			}
		}
	}



	//�ʒu�̍X�V
	g_Player2.pos.x += g_Player2.move.x;
	g_Player2.pos.y += g_Player2.move.y;

	//�ړ��ʂ̍X�V(����)
	g_Player2.move.x += (PLAYER_SPEED_DEF - g_Player2.move.x) * PLAYER_INA;
	g_Player2.move.y += (PLAYER_SPEED_DEF - g_Player2.move.y) * PLAYER_INA;

	g_Player2.nCounterAnim++;
	if ((g_Player2.nCounterAnim % UP_TIME) == 0)//�X�V����
	{
		g_Player2.nPatternAnim = (g_Player2.nPatternAnim + 1) % (U_MAX * V_MAX);

		g_pVtxBuffPlayer2->Lock(0, 0, (void**)&pVtx, 0);//�v���C���o�b�t�@�̃��b�N

	//���W�ݒ�
		pVtx[0].pos.x = g_Player2.pos.x + sinf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength;
		pVtx[0].pos.y = g_Player2.pos.y + cosf(g_Player2.rot.z - (D3DX_PI - g_Player2.fAngle)) * g_Player2.fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_Player2.pos.x + sinf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength;
		pVtx[1].pos.y = g_Player2.pos.y + cosf(g_Player2.rot.z - (D3DX_PI + g_Player2.fAngle)) * g_Player2.fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = g_Player2.pos.x + sinf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength;
		pVtx[2].pos.y = g_Player2.pos.y + cosf(g_Player2.rot.z - g_Player2.fAngle) * g_Player2.fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = g_Player2.pos.x + sinf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength;
		pVtx[3].pos.y = g_Player2.pos.y + cosf(g_Player2.rot.z + g_Player2.fAngle) * g_Player2.fLength;
		pVtx[3].pos.z = 0.0f;

		//�e�N�X�`��
		pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player2.nPatternAnim, UV_DEF / V_MAX * (g_Player2.nPatternAnim / U_MAX));
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player2.nPatternAnim + UV_DEF / U_MAX, UV_DEF / V_MAX * (g_Player2.nPatternAnim / U_MAX));
		pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player2.nPatternAnim, UV_DEF / V_MAX * (g_Player2.nPatternAnim / U_MAX) + UV_DEF / V_MAX);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX * g_Player2.nPatternAnim + UV_DEF / U_MAX, UV_DEF / V_MAX * (g_Player2.nPatternAnim / U_MAX) + UV_DEF / V_MAX);


		if (pVtx[0].pos.x >= SCREEN_WIDTH || pVtx[1].pos.x >= SCREEN_WIDTH || pVtx[2].pos.x >= SCREEN_WIDTH || pVtx[3].pos.x >= SCREEN_WIDTH)//��ʉE�[
		{
			g_Player2.pos.x = g_Player2.fLength;//��ʍ��[
		}
		else if (pVtx[0].pos.x <= 0 || pVtx[1].pos.x <= 0 || pVtx[2].pos.x <= 0 || pVtx[3].pos.x <= 0)//��ʍ��[
		{
			g_Player2.pos.x = SCREEN_WIDTH - g_Player2.fLength;//��ʉE�[
		}

		if (pVtx[0].pos.y >= SCREEN_HEIGHT || pVtx[1].pos.y >= SCREEN_HEIGHT || pVtx[2].pos.y >= SCREEN_HEIGHT || pVtx[3].pos.y >= SCREEN_HEIGHT)//��ʉ��[
		{
			g_Player2.pos.y = g_Player2.fLength;//��ʏ�[
		}
		else if (pVtx[0].pos.y <= 0 || pVtx[1].pos.y <= 0 || pVtx[2].pos.y <= 0 || pVtx[3].pos.y <= 0)
		{
			g_Player2.pos.y = SCREEN_HEIGHT - g_Player2.fLength;//��ʉ��[
		}

		switch (g_Player2.state)
		{
		case PLAYERSTATE_NORMAL:
			SetVibrate(0.0f, CONTROLLER_1);
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case PLAYERSTATE_APPEAR:
			g_Player2.nCounterState++;
			if (g_Player2.nCounterState >= 60)
			{
				g_Player2.state = PLAYERSTATE_NORMAL;
				g_Player2.bDisp = true;
			}
			else if (g_Player2.nCounterState % 5 == 0)
			{
				g_Player2.bDisp = !g_Player2.bDisp;
			}
			break;
		case PLAYERSTATE_DAMAGE:
			g_Player2.nCounterState--;
			if (g_Player2.nCounterState <= 0)
			{
				g_Player2.state = PLAYERSTATE_NORMAL;

				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			}
			break;
		case PLAYERSTATE_DIE:
			g_Player2.bDisp = false;
			break;
		}

		g_pVtxBuffPlayer2->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
	}
}

//-------------------
//�v���C���[�`�揈��
//-------------------
void DrawPlayer2(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	if (g_Player2.bDisp)
	{
		//���_�o�b�t�@
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer2, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer2);

		//�v���C���[�̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//�^�C�v
			0,//�n�܂�̔ԍ�
			2//�|���S���̌�
		);
	}
}

//-------------------
//�q�b�g����
//-------------------
void HitPlayer2(int nDamage)
{
	if (g_Player2.state != PLAYERSTATE_DIE && g_Player2.state != PLAYERSTATE_APPEAR)
	{
		VERTEX_2D* pVtx;

		g_Player2.nLife -= nDamage;

		if (g_Player2.nLife <= 0)
		{
			if (g_Player2.nRema <= 0)
			{
				//SetExplosion(g_Player2.pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_Player2.fLength);
				SetExef(g_Player2.pos,g_Player2.fLength);
				g_Player2.state = PLAYERSTATE_DIE;
			}
			else
			{
				//SetExplosion(g_Player2.pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_Player2.fLength);

				SetExef(g_Player2.pos, g_Player2.fLength);
				g_Player2.state = PLAYERSTATE_APPEAR;
				g_Player2.nCounterState = 0;
				g_Player2.nRema--;
				g_Player2.nLife = 100;
				g_Player2.pos = D3DXVECTOR3(SCREEN_WIDTH-PLAYER_WIDTH, SCREEN_HEIGHT / 2, 0);//�����ʒu
			}
			SetVibrate(1.0f, CONTROLLER_2);
		}
		else
		{
			g_Player2.state = PLAYERSTATE_DAMAGE;
			g_Player2.nCounterState = 5;

			g_pVtxBuffPlayer2->Lock(0, 0, (void**)&pVtx, 0);
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			g_pVtxBuffPlayer2->Unlock();//�o�b�t�@�̃A�����b�N
		}
	}
}

//-------------------------
//�v���C���[�o�b�t�@�̎擾
//-------------------------
LPDIRECT3DVERTEXBUFFER9 GetPlayerBuffer2(void)
{
	return g_pVtxBuffPlayer2;
}

//-------------------------
//�v���C���[�擾
//-------------------------
Player* GetPlayer2(void)
{
	return &g_Player2;
}