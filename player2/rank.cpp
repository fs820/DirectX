//------------------------------------------
//
//�����L���O�̏���[rank.cpp]
//Author fuma sato
//
//------------------------------------------

#include"rank.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
#include"score.h"
#include"data.h"
#include"cursor.h"
#include<assert.h>

#define SELECT_WIDTH (512)
#define SELECT_HEIGHT (128)
#define RANK_MAX (5)

typedef enum
{
	RANKSTATE_NORMAL = 0,
	RANKSTATE_SELECT,
	RANKSTATE_MAX
}RANKSTATE;

typedef enum
{
	SELECT_BACK = 0,
	SELECT_RETRY,
	SELECT_EXIT,
	SELECT_MAX,
}SELECT;

LPDIRECT3DTEXTURE9 g_apTextureRank[RANK_MAX] = { NULL };//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;//�o�b�t�@�̃|�C���^
RANKSTATE g_RankState = RANKSTATE_NORMAL;

//--------------------
//����������
//--------------------
void InitRank(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^
	D3DXVECTOR3 posScore;//�X�R�A�̈ʒu
	D3DXVECTOR3 posScoreDef;//�X�R�A�̈ʒu
	int nScore[MAX_DATA+1] = { 0 };
	int aPosTexUr[MAX_DATA+1][SCORE_MAX];
	int* pData;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * ((SCORE_MAX*MAX_DATA) + 4),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_RESULT,
		&g_apTextureRank[0]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SCORE,
		&g_apTextureRank[1]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTBACK,
		&g_apTextureRank[2]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTRETRY,
		&g_apTextureRank[3]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTEXIT,
		&g_apTextureRank[4]
	);

	//�ߋ��̃X�R�A�̎擾
	pData = LoadData();
	for (int i = 0; i < MAX_DATA; i++,pData++)
	{
		nScore[i] = *pData;
	}

	//����̃X�R�A�̎擾
	nScore[MAX_DATA] = GetScore();
	pData = Soat(&nScore[0]);
	for (int i = 0; i < MAX_DATA; i++, pData++)
	{
		nScore[i] = *pData;
	}
	//�Z�[�u����
	SaveData(&nScore[0]);

	//�\������
	for (int i = 0; i < MAX_DATA; i++)
	{
		for (int i2 = 0; i2 < SCORE_MAX; i2++)
		{
			aPosTexUr[i][i2] = Digit(nScore[i], i2);
		}
	}

	posScore = D3DXVECTOR3(SCREEN_WIDTH / 2 - (SCORE_WIDTH / U_MAX_S) * (SCORE_MAX / 2), SCORE_HEIGHT/2, 0.0f);
	posScoreDef = posScore;
	D3DXVECTOR3 posSelect = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - SELECT_HEIGHT, 0.0f);
	g_RankState = RANKSTATE_NORMAL;

	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

		//���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += VT_MAX;
	for (int i = 0; i < MAX_DATA; i++)
	{
		for (int i2 = 0; i2 < SCORE_MAX; i2++)
		{
			//���W�ݒ�
			pVtx[0].pos = D3DXVECTOR3(posScore.x - (SCORE_WIDTH / U_MAX_S) / 2, posScore.y - (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);
			pVtx[1].pos = D3DXVECTOR3(posScore.x + (SCORE_WIDTH / U_MAX_S) / 2, posScore.y - (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);
			pVtx[2].pos = D3DXVECTOR3(posScore.x - (SCORE_WIDTH / U_MAX_S) / 2, posScore.y + (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);
			pVtx[3].pos = D3DXVECTOR3(posScore.x + (SCORE_WIDTH / U_MAX_S) / 2, posScore.y + (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);

			//rhw
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			//�e�N�X�`��
			pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i][i2], UV_DEF / V_MAX_S * (aPosTexUr[i][i2] / U_MAX_S));
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i][i2] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexUr[i][i2] / U_MAX_S));
			pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i][i2], UV_DEF / V_MAX_S * (aPosTexUr[i][i2] / U_MAX_S) + UV_DEF / V_MAX_S);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i][i2] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexUr[i][i2] / U_MAX_S) + UV_DEF / V_MAX_S);

			posScore.x += SCORE_WIDTH / U_MAX_S;
			pVtx += VT_MAX;
		}
		posScore = posScoreDef;
		posScore.y += SCORE_HEIGHT * (i+1);
	}

	for (int i = 0; i < SELECT_MAX; i++)
	{

		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[1].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[2].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[3].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);

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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += VT_MAX;
	}

	g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
	PlaySound(SOUND_LABEL_BGM6);
}

//--------------------
//�I������
//--------------------
void UninitRank(void)
{
	//�T�E���h
	StopSound();
	//�e�N�X�`���̔j��
	for (int i = 0; i < RANK_MAX; i++)
	{
		if (g_apTextureRank[i] != NULL)
		{
			g_apTextureRank[i]->Release();
			g_apTextureRank[i] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRank != NULL)
	{
		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}
}

//--------------------
//�X�V����
//--------------------
void UpdateRank(void)
{
	static SELECT SelectNew = SELECT_BACK;
	if (g_RankState==RANKSTATE_NORMAL)
	{
		if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_START) == true||GetMouseTrigger(MOUSE_LEFT)==true)
		{
			FADE fade;
			fade = GetFade();
			if (fade == FADE_NONE)
			{
				g_RankState = RANKSTATE_SELECT;
				VERTEX_2D* pVtx;//���_���|�C���^

				g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

				//�J���[
				pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

			}
		}
	}
	else
	{
		static bool bStick = false;

		if (GetKeyboradTrigger(DIK_BACK) == true || GetJoykeyTrigger(JOYKEY_B) == true||GetMouseTrigger(MOUSE_RIGHT) == true)
		{
			g_RankState = RANKSTATE_NORMAL;
		}
		else if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_A) == true || GetMouseTrigger(MOUSE_SENTER) == true)
		{
			FADE fade;
			switch (SelectNew)
			{
			case SELECT_BACK:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//�T�E���h
					StopSound();
					//�ؑ�
					SetFade(MODE_TITLE);
				}
				break;
			case SELECT_RETRY:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//�T�E���h
					StopSound();
					//�ؑ�
					SetFade(MODE_GAME);
				}
				break;
			case SELECT_EXIT:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//�T�E���h
					StopSound();
					//�ؑ�
					HWND hWnd;
					hWnd = GethWnd();
					PostMessage(hWnd, WM_QUIT, 0, 0);
				}
				break;
			}
		}
		else if (GetMouseTrigger(MOUSE_LEFT) == true)
		{
			VERTEX_2D* pVtx;//���_���|�C���^
			CURSOR* pCursor;
			pCursor = GetCursorIn();

			g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

			pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1);

			for (int i = 0; i < SELECT_MAX; i++)
			{
				if (pCursor->pos.x >= pVtx[0].pos.x && pCursor->pos.x <= pVtx[3].pos.x && pCursor->pos.y >= pVtx[0].pos.y && pCursor->pos.y <= pVtx[3].pos.y)
				{
					FADE fade;
					switch (i)
					{
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound();
							//�ؑ�
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound();
							//�ؑ�
							SetFade(MODE_GAME);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//�T�E���h
							StopSound();
							//�ؑ�
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
					break;
				}
				pVtx += VT_MAX;
			}

			g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
		}
		else if (bStick==false)
		{
			if (GetKeyboradTrigger(DIK_UP) == true || GetKeyboradTrigger(DIK_W) == true || GetJoykeyTrigger(JOYKEY_UP) == true|| *(GetJoyStick(STICK_LEFT) + 1) > STICK_DED||GetMouseWheel()>0.0f)
			{
				VERTEX_2D* pVtx;//���_���|�C���^

				g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

				switch (SelectNew)
				{
				case SELECT_BACK:
					SelectNew = SELECT_EXIT;
					break;
				case SELECT_RETRY:
					SelectNew = SELECT_BACK;
					break;
				case SELECT_EXIT:
					SelectNew = SELECT_RETRY;
					break;
				}

				g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

				//�J���[
				pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
			}
			else if (GetKeyboradTrigger(DIK_DOWN) == true || GetKeyboradTrigger(DIK_S) == true || GetJoykeyTrigger(JOYKEY_DOWN) == true || *(GetJoyStick(STICK_LEFT) + 1) < -STICK_DED || GetMouseWheel() < 0.0f)
			{
				VERTEX_2D* pVtx;//���_���|�C���^

				g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

				switch (SelectNew)
				{
				case SELECT_BACK:
					SelectNew = SELECT_RETRY;
					break;
				case SELECT_RETRY:
					SelectNew = SELECT_EXIT;
					break;
				case SELECT_EXIT:
					SelectNew = SELECT_BACK;
					break;
				}
				g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

				//�J���[
				pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
			}
		}

		if (*(GetJoyStick(STICK_LEFT) + 1) > STICK_DED || *(GetJoyStick(STICK_LEFT) + 1) < -STICK_DED)
		{
			bStick = true;
		}
		else
		{
			bStick = false;
		}
	}

	for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
	{
		if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
		{

		}
		else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
		{

		}
		else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
		{

		}
		else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
		{

		}
	}


	if (!IsXInputControllerConnected()&&IsDirectInputControllerConnected())
	{
		if (g_RankState == RANKSTATE_NORMAL)
		{
			if (GetdJoykeyTrigger(11) == true)
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					g_RankState = RANKSTATE_SELECT;
					VERTEX_2D* pVtx;//���_���|�C���^

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

				}
			}
		}
		else
		{
			static bool bStick = false;

			if (GetdJoykeyTrigger(10) == true)
			{
				g_RankState = RANKSTATE_NORMAL;
			}
			else if (GetdJoykeyTrigger(0) == true)
			{
				FADE fade;
				switch (SelectNew)
				{
				case SELECT_BACK:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//�T�E���h
						StopSound();
						//�ؑ�
						SetFade(MODE_TITLE);
					}
					break;
				case SELECT_RETRY:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//�T�E���h
						StopSound();
						//�ؑ�
						SetFade(MODE_GAME);
					}
					break;
				case SELECT_EXIT:
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//�T�E���h
						StopSound();
						//�ؑ�
						HWND hWnd;
						hWnd = GethWnd();
						PostMessage(hWnd, WM_QUIT, 0, 0);
					}
					break;
				}
			}
			else if (bStick == false)
			{
				if (GetdJoyPovTrigger(POV_UP, 0) == true || *(GetdJoyStick(STICK_LEFT) + 1) < -STICK_DED)
				{
					VERTEX_2D* pVtx;//���_���|�C���^

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (SelectNew)
					{
					case SELECT_BACK:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_BACK;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_RETRY;
						break;
					}

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0) == true || *(GetdJoyStick(STICK_LEFT) + 1) > STICK_DED)
				{
					VERTEX_2D* pVtx;//���_���|�C���^

					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (SelectNew)
					{
					case SELECT_BACK:
						SelectNew = SELECT_RETRY;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_BACK;
						break;
					}
					g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * (SCORE_MAX * MAX_DATA + 1 + SelectNew);

					//�J���[
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffRank->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
			}

			if (*(GetdJoyStick(STICK_LEFT) + 1) < -STICK_DED || *(GetdJoyStick(STICK_LEFT) + 1) > STICK_DED)
			{
				bStick = true;
			}
			else
			{
				bStick = false;
			}
		}
	}
}

//--------------------
//�`�揈��
//--------------------
void DrawRank(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureRank[0]);

	//�w�i�̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//�^�C�v
		0,//�n�܂�̔ԍ�
		2//�|���S���̌�
	);

	int nCount=0;
	for (int i = 0; i < MAX_DATA; i++)
	{
		for (int i2 = 0; i2 < SCORE_MAX; i2++)
		{
			nCount++;
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureRank[1]);

			//�w�i�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				nCount * VT_MAX,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}

	if (g_RankState==RANKSTATE_SELECT)
	{
		for (int i = 0; i < SELECT_MAX; i++)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureRank[i + 2]);

			//�w�i�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				i * VT_MAX + (VT_MAX * (SCORE_MAX * MAX_DATA + 1)),//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}
}