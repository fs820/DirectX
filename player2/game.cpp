//------------------------------------------
//
//�Q�[�����C���̏���[game.cpp]
//Author fuma sato
//
//------------------------------------------

#include"game.h"
#include"player.h"
#include"back.h"
#include"bullet.h"
#include"effect.h"
#include"explosion.h"
#include"enemy.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
#include"score.h"
#include"cursor.h"

#define SELECT_WIDTH (512)
#define SELECT_HEIGHT (128)

typedef enum
{
	SELECT_RESTART = 0,
	SELECT_RETRY,
	SELECT_BACK,
	SELECT_EXIT,
	SELECT_MAX,
}SELECT;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureGame[SELECT_MAX] = { NULL };//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;//�o�b�t�@�̃|�C���^
GAMESTATE g_gameState = GAMESTATE_NONE;
int g_nCounterGameState = 0;

//--------------------
//����������
//--------------------
void InitGame(void)
{
	InitBack();//�w�i
	InitBullet();//�e
	InitPlayer();//�v���C���[
	InitEnemy();//�G
	InitEffect();
	InitExplosion();//����
	InitScore();//�X�R�A
	SetScore(0);//�X�R�A��������
	SetEnemy(D3DXVECTOR3(1000.0f, 200.0f, 0.0f), 0);//�G����
	SetEnemy(D3DXVECTOR3(1000.0f, 500.0f, 0.0f), 1);//�G����
	SetEnemy(D3DXVECTOR3(1000.0f, 350.0f, 0.0f), 2);//�G����
	SetEnemy(D3DXVECTOR3(700.0f, 350.0f, 0.0f), 3);//�G����

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^
	D3DXVECTOR3 posSelect;//�X�R�A�̈ʒu

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�[�̐ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * SELECT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGame,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTRESTART,
		&g_apTextureGame[0]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTRETRY,
		&g_apTextureGame[1]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTBACK,
		&g_apTextureGame[2]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTEXIT,
		&g_apTextureGame[3]
	);


	g_gameState = GAMESTATE_NORMAL;
	g_nCounterGameState = 0;
	posSelect = D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2-SELECT_HEIGHT*1.5f,0.0f);

	g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

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

	g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

	PlaySound(SOUND_LABEL_BGM4);
}

//------------------
//�I������
//------------------
void UninitGame(void)
{
	g_gameState = GAMESTATE_NONE;

	//�T�E���h
	StopSound();

	UninitScore();//�X�R�A
	UninitExplosion();//����
	UninitEffect();
	UninitEnemy();//�G
	UninitPlayer();//�v���C���[
	UninitBullet();//�e
	UninitBack();//�w�i
}

//--------------
//�X�V����
//--------------
void UpdateGame(void)
{
	static SELECT SelectNew = SELECT_RESTART;

	if (g_gameState!=GAMESTATE_PAUSE)
	{
		if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_START) == true||(GetMousePress(MOUSE_LEFT) == true&& GetMousePress(MOUSE_RIGHT) == true&& GetMousePress(MOUSE_SENTER) == true))
		{
			FADE fade;
			fade = GetFade();
			if (fade == FADE_NONE)
			{
				g_gameState = GAMESTATE_PAUSE;
				VERTEX_2D* pVtx;//���_���|�C���^

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

				StopSound();
			}
		}

		int NumEnemy;
		Player* pPlayer;

		pPlayer = GetPlayer();
		NumEnemy = GetNumEnemy();

		switch (g_gameState)
		{
		case GAMESTATE_NORMAL:
			if (pPlayer->state == PLAYERSTATE_DIE)
			{
				SetGameState(GAMESTATE_END);
			}
			else if (NumEnemy <= 0)
			{
				SetGameState(GAMESTATE_END);
			}
			break;
		case GAMESTATE_END:
			g_nCounterGameState++;
			if (g_nCounterGameState >= INTER)
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//�T�E���h
					StopSound();
					//�ؑ�
					SetFade(MODE_RESULT);
				}
			}
			break;
		}
	}
	else
	{
		static bool bStick = false;

		if (GetKeyboradTrigger(DIK_BACK) == true || GetJoykeyTrigger(JOYKEY_B) == true || GetMouseTrigger(MOUSE_RIGHT) == true)
		{
			g_gameState = GAMESTATE_NORMAL;
			PlaySound(SOUND_LABEL_BGM4);
		}
		else if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_A) == true || GetMouseTrigger(MOUSE_SENTER) == true)
		{
			FADE fade;
			switch (SelectNew)
			{
			case SELECT_RESTART:
				g_gameState = GAMESTATE_NORMAL;
				PlaySound(SOUND_LABEL_BGM4);
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

			g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

			for (int i = 0; i < SELECT_MAX; i++)
			{
				if (pCursor->pos.x >= pVtx[0].pos.x && pCursor->pos.x <= pVtx[3].pos.x && pCursor->pos.y >= pVtx[0].pos.y && pCursor->pos.y <= pVtx[3].pos.y)
				{
					FADE fade;
					switch (i)
					{
					case SELECT_RESTART:
						g_gameState = GAMESTATE_NORMAL;
						PlaySound(SOUND_LABEL_BGM4);
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
				pVtx += VT_MAX;
			}

			g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
		}
		else if (bStick==false)
		{
			if (GetKeyboradTrigger(DIK_UP) == true || GetKeyboradTrigger(DIK_W) == true || GetJoykeyTrigger(JOYKEY_UP) == true || *(GetJoyStick(STICK_LEFT) + 1) > STICK_DED || GetMouseWheel() > 0.0f)
			{
				VERTEX_2D* pVtx;//���_���|�C���^

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

				switch (SelectNew)
				{
				case SELECT_RESTART:
					SelectNew = SELECT_EXIT;
					break;
				case SELECT_RETRY:
					SelectNew = SELECT_RESTART;
					break;
				case SELECT_BACK:
					SelectNew = SELECT_RETRY;
					break;
				case SELECT_EXIT:
					SelectNew = SELECT_BACK;
					break;
				}

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
			}
			else if (GetKeyboradTrigger(DIK_DOWN) == true || GetKeyboradTrigger(DIK_S) == true || GetJoykeyTrigger(JOYKEY_DOWN) == true || *(GetJoyStick(STICK_LEFT) + 1) < -STICK_DED || GetMouseWheel() < 0.0f)
			{
				VERTEX_2D* pVtx;//���_���|�C���^

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

				switch (SelectNew)
				{
				case SELECT_RESTART:
					SelectNew = SELECT_RETRY;
					break;
				case SELECT_RETRY:
					SelectNew = SELECT_BACK;
					break;
				case SELECT_BACK:
					SelectNew = SELECT_EXIT;
					break;
				case SELECT_EXIT:
					SelectNew = SELECT_RESTART;
					break;
				}
				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

				pVtx += VT_MAX * SelectNew;

				//�J���[
				pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
			}
		}

		if (*(GetJoyStick(STICK_LEFT) + 1) > STICK_DED || *(GetJoyStick(STICK_LEFT) + 1) < -STICK_DED)
		{
			bStick=true;
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
		if (g_gameState != GAMESTATE_PAUSE)
		{
			if (GetdJoykeyTrigger(11) == true)
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					g_gameState = GAMESTATE_PAUSE;
					VERTEX_2D* pVtx;//���_���|�C���^

					g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * SelectNew;

					//�J���[
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					StopSound();
				}
			}

			int NumEnemy;
			Player* pPlayer;

			pPlayer = GetPlayer();
			NumEnemy = GetNumEnemy();

			switch (g_gameState)
			{
			case GAMESTATE_NORMAL:
				if (pPlayer->state == PLAYERSTATE_DIE)
				{
					SetGameState(GAMESTATE_END);
				}
				else if (NumEnemy <= 0)
				{
					SetGameState(GAMESTATE_END);
				}
				break;
			case GAMESTATE_END:
				g_nCounterGameState++;
				if (g_nCounterGameState >= INTER)
				{
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						//�T�E���h
						StopSound();
						//�ؑ�
						SetFade(MODE_RESULT);
					}
				}
				break;
			}
		}
		else
		{
			static bool bStick = false;

			if (GetdJoykeyTrigger(10) == true)
			{
				g_gameState = GAMESTATE_NORMAL;
				PlaySound(SOUND_LABEL_BGM4);
			}
			else if (GetdJoykeyTrigger(0) == true)
			{
				FADE fade;
				switch (SelectNew)
				{
				case SELECT_RESTART:
					g_gameState = GAMESTATE_NORMAL;
					PlaySound(SOUND_LABEL_BGM4);
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

					g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * SelectNew;

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (SelectNew)
					{
					case SELECT_RESTART:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_RESTART;
						break;
					case SELECT_BACK:
						SelectNew = SELECT_RETRY;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_BACK;
						break;
					}

					g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * SelectNew;

					//�J���[
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0) == true || *(GetdJoyStick(STICK_LEFT) + 1) > STICK_DED)
				{
					VERTEX_2D* pVtx;//���_���|�C���^

					g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * SelectNew;

					//�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N

					switch (SelectNew)
					{
					case SELECT_RESTART:
						SelectNew = SELECT_RETRY;
						break;
					case SELECT_RETRY:
						SelectNew = SELECT_BACK;
						break;
					case SELECT_BACK:
						SelectNew = SELECT_EXIT;
						break;
					case SELECT_EXIT:
						SelectNew = SELECT_RESTART;
						break;
					}
					g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//�v���C���[�o�b�t�@�̃��b�N

					pVtx += VT_MAX * SelectNew;

					//�J���[
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffGame->Unlock();//�v���C���[�o�b�t�@�̃A�����b�N
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

	if (g_gameState != GAMESTATE_PAUSE)
	{
		UpdateBack();//�w�i
		UpdateBullet();//�e
		UpdatePlayer();//�v���C���[
		UpdateEnemy();//�G
		UpdateEffect();
		UpdateExplosion();//����
		UpdateScore();//�X�R�A
	}
}

//-------------------
//�`�揈��
//-------------------
void DrawGame(void)
{

	DrawBack();//�w�i
	DrawBullet();//�e
	DrawPlayer();//�v���C���[
	DrawEnemy();//�G
	DrawEffect();
	DrawExplosion();//����
	DrawScore();//�X�R�A

	if(g_gameState==GAMESTATE_PAUSE)
	{
		LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

        //�f�o�C�X�̎擾
		pDevice = GetDevice();

		//���_�o�b�t�@
		pDevice->SetStreamSource(0, g_pVtxBuffGame, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < SELECT_MAX; i++)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureGame[i]);

			//�w�i�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				i*VT_MAX,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}
}

//----------------------
//�Q�[����Ԑؑ�
//----------------------
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//-----------------------
//�Q�[����Ԏ擾
//-----------------------
GAMESTATE GetGameState(void)
{
	return g_gameState;
}