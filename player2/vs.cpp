//------------------------------------------
//
//ゲームメインの処理[vs.cpp]
//Author fuma sato
//
//------------------------------------------

#include"vs.h"
#include"player.h"
#include"player2.h"
#include"back.h"
#include"bullet.h"
#include"effect.h"
#include"exef.h"
#include"explosion.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
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

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureVs[SELECT_MAX] = { NULL };//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffVs = NULL;//バッファのポインタ
VSSTATE g_VsState = VSSTATE_NONE;
int g_nCounterVsState = 0;

//--------------------
//初期化処理
//--------------------
void InitVs(void)
{
	InitBack();//背景
	InitBullet();//弾
	InitPlayer();//プレイヤー
	InitPlayer2();
	InitEffect();
	InitExef();
	InitExplosion();//爆発

	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ
	D3DXVECTOR3 posSelect;//スコアの位置

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * SELECT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffVs,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTRESTART,
		&g_apTextureVs[0]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTRETRY,
		&g_apTextureVs[1]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTBACK,
		&g_apTextureVs[2]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTEXIT,
		&g_apTextureVs[3]
	);


	g_VsState = VSSTATE_NORMAL;
	g_nCounterVsState = 0;
	posSelect = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - SELECT_HEIGHT * 1.5f, 0.0f);

	g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	for (int i = 0; i < SELECT_MAX; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[1].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y - SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[2].pos = D3DXVECTOR3(posSelect.x - SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);
		pVtx[3].pos = D3DXVECTOR3(posSelect.x + SELECT_WIDTH / 2, posSelect.y + SELECT_HEIGHT / 2 + i * SELECT_HEIGHT, posSelect.z);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += VT_MAX;

	}

	g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

	PlaySound(SOUND_LABEL_BGM4);
}

//------------------
//終了処理
//------------------
void UninitVs(void)
{
	g_VsState = VSSTATE_NONE;

	//サウンド
	StopSound();

	UninitExplosion();//爆発
	UninitExef();
	UninitEffect();
	UninitPlayer2();
	UninitPlayer();//プレイヤー
	UninitBullet();//弾
	UninitBack();//背景
}

//--------------
//更新処理
//--------------
void UpdateVs(void)
{
	static SELECT SelectNew = SELECT_RESTART;
	static int SpoEneCount = 0;

	if (g_VsState != VSSTATE_PAUSE)
	{
		if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_START, CONTROLLER_MAX) == true || (GetMousePress(MOUSE_LEFT) == true && GetMousePress(MOUSE_RIGHT) == true && GetMousePress(MOUSE_SENTER) == true))
		{
			FADE fade;
			fade = GetFade();
			if (fade == FADE_NONE)
			{
				g_VsState = VSSTATE_PAUSE;
				VERTEX_2D* pVtx;//頂点情報ポインタ

				g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * SelectNew;

				//カラー
				pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

				StopSound();
			}
		}

		Player* pPlayer,*pPlayer2;

		pPlayer = GetPlayer();
		pPlayer2 = GetPlayer2();

		switch (g_VsState)
		{
		case VSSTATE_NORMAL:
			if (pPlayer->state == PLAYERSTATE_DIE)
			{
				SetVsState(VSSTATE_END);
			}
			else if (pPlayer2->state == PLAYERSTATE_DIE)
			{
				SetVsState(VSSTATE_END);
			}
			break;
		case VSSTATE_END:
			g_nCounterVsState++;
			if (g_nCounterVsState >= INTER)
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_TITLE);
				}
			}
			break;
		}
	}
	else
	{
		if (GetKeyboradTrigger(DIK_BACK) == true || GetJoykeyTrigger(JOYKEY_B, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_RIGHT) == true)
		{
			g_VsState = VSSTATE_NORMAL;
			PlaySound(SOUND_LABEL_BGM4);
		}
		else if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_A, CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_SENTER) == true)
		{
			FADE fade;
			switch (SelectNew)
			{
			case SELECT_RESTART:
				g_VsState = VSSTATE_NORMAL;
				PlaySound(SOUND_LABEL_BGM4);
				break;
			case SELECT_RETRY:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_VS);
				}
				break;
			case SELECT_BACK:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_TITLE);
				}
				break;
			case SELECT_EXIT:
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					HWND hWnd;
					hWnd = GethWnd();
					PostMessage(hWnd, WM_QUIT, 0, 0);
				}
				break;
			}
		}
		else if (GetMouseTrigger(MOUSE_LEFT) == true)
		{
			VERTEX_2D* pVtx;//頂点情報ポインタ
			CURSOR* pCursor;
			pCursor = GetCursorIn();

			g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			for (int i = 0; i < SELECT_MAX; i++)
			{
				if (pCursor->pos.x >= pVtx[0].pos.x && pCursor->pos.x <= pVtx[3].pos.x && pCursor->pos.y >= pVtx[0].pos.y && pCursor->pos.y <= pVtx[3].pos.y)
				{
					FADE fade;
					switch (i)
					{
					case SELECT_RESTART:
						g_VsState = VSSTATE_NORMAL;
						PlaySound(SOUND_LABEL_BGM4);
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound();
							//切替
							SetFade(MODE_VS);
						}
						break;
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound();
							//切替
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound();
							//切替
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
				}
				pVtx += VT_MAX;
			}

			g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック
		}
		else if (GetKeyboradTrigger(DIK_UP) == true || GetKeyboradTrigger(DIK_W) == true || GetJoykeyTrigger(JOYKEY_UP, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_UP, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() > 0.0f)
		{
			VERTEX_2D* pVtx;//頂点情報ポインタ

			g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * SelectNew;

			//カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

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

			g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * SelectNew;

			//カラー
			pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

			g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック
		}
		else if (GetKeyboradTrigger(DIK_DOWN) == true || GetKeyboradTrigger(DIK_S) == true || GetJoykeyTrigger(JOYKEY_DOWN, CONTROLLER_MAX) == true || JoyStickTrigger(DIRESTICK_DOWN, STICK_LEFT, CONTROLLER_MAX) || GetMouseWheel() < 0.0f)
		{
			VERTEX_2D* pVtx;//頂点情報ポインタ

			g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * SelectNew;

			//カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

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
			g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

			pVtx += VT_MAX * SelectNew;

			//カラー
			pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

			g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック
		}
	}

	for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
	{
		if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
		{
			if (g_VsState != VSSTATE_PAUSE)
			{
				if (GetdJoykeyTrigger(ELEKEY_START, (CONTROLLER)i) == true)
				{
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_VsState = VSSTATE_PAUSE;
						VERTEX_2D* pVtx;//頂点情報ポインタ

						g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

						g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

						StopSound();
					}
				}
			}
			else
			{
				if (GetdJoykeyTrigger(ELEKEY_BACK, (CONTROLLER)i) == true)
				{
					g_VsState = VSSTATE_NORMAL;
					PlaySound(SOUND_LABEL_BGM4);
				}
				else if (GetdJoykeyTrigger(ELEKEY_A, (CONTROLLER)i) == true)
				{
					FADE fade;
					switch (SelectNew)
					{
					case SELECT_RESTART:
						g_VsState = VSSTATE_NORMAL;
						PlaySound(SOUND_LABEL_BGM4);
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound();
							//切替
							SetFade(MODE_VS);
						}
						break;
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound();
							//切替
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound();
							//切替
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
				}
				else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

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

					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

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
					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
		{
			if (g_VsState != VSSTATE_PAUSE)
			{
				if (GetdJoykeyTrigger(PSKEY_START, (CONTROLLER)i) == true)
				{
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_VsState = VSSTATE_PAUSE;
						VERTEX_2D* pVtx;//頂点情報ポインタ

						g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

						g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

						StopSound();
					}
				}
			}
			else
			{
				if (GetdJoykeyTrigger(PSKEY_BACK, (CONTROLLER)i) == true)
				{
					g_VsState = VSSTATE_NORMAL;
					PlaySound(SOUND_LABEL_BGM4);
				}
				else if (GetdJoykeyTrigger(PSKEY_CI, (CONTROLLER)i) == true)
				{
					FADE fade;
					switch (SelectNew)
					{
					case SELECT_RESTART:
						g_VsState = VSSTATE_NORMAL;
						PlaySound(SOUND_LABEL_BGM4);
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound();
							//切替
							SetFade(MODE_VS);
						}
						break;
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound();
							//切替
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound();
							//切替
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
				}
				else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

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

					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

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
					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
		{
			if (g_VsState != VSSTATE_PAUSE)
			{
				if (GetdJoykeyTrigger(NINKEY_＋, (CONTROLLER)i) == true)
				{
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_VsState = VSSTATE_PAUSE;
						VERTEX_2D* pVtx;//頂点情報ポインタ

						g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

						g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

						StopSound();
					}
				}
			}
			else
			{
				if (GetdJoykeyTrigger(NINKEY_－, (CONTROLLER)i) == true)
				{
					g_VsState = VSSTATE_NORMAL;
					PlaySound(SOUND_LABEL_BGM4);
				}
				else if (GetdJoykeyTrigger(NINKEY_A, (CONTROLLER)i) == true)
				{
					FADE fade;
					switch (SelectNew)
					{
					case SELECT_RESTART:
						g_VsState = VSSTATE_NORMAL;
						PlaySound(SOUND_LABEL_BGM4);
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound();
							//切替
							SetFade(MODE_VS);
						}
						break;
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound();
							//切替
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound();
							//切替
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
				}
				else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

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

					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

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
					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック
				}
			}
		}
		else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
		{
			if (g_VsState != VSSTATE_PAUSE)
			{
				if (GetdJoykeyTrigger(DKEY_START, (CONTROLLER)i) == true)
				{
					FADE fade;
					fade = GetFade();
					if (fade == FADE_NONE)
					{
						g_VsState = VSSTATE_PAUSE;
						VERTEX_2D* pVtx;//頂点情報ポインタ

						g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

						pVtx += VT_MAX * SelectNew;

						//カラー
						pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

						g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

						StopSound();
					}
				}
			}
			else
			{
				if (GetdJoykeyTrigger(DKEY_BACK, (CONTROLLER)i) == true)
				{
					g_VsState = VSSTATE_NORMAL;
					PlaySound(SOUND_LABEL_BGM4);
				}
				else if (GetdJoykeyTrigger(DKEY_A, (CONTROLLER)i) == true)
				{
					FADE fade;
					switch (SelectNew)
					{
					case SELECT_RESTART:
						g_VsState = VSSTATE_NORMAL;
						PlaySound(SOUND_LABEL_BGM4);
						break;
					case SELECT_RETRY:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound();
							//切替
							SetFade(MODE_VS);
						}
						break;
					case SELECT_BACK:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound();
							//切替
							SetFade(MODE_TITLE);
						}
						break;
					case SELECT_EXIT:
						fade = GetFade();
						if (fade == FADE_NONE)
						{
							//サウンド
							StopSound();
							//切替
							HWND hWnd;
							hWnd = GethWnd();
							PostMessage(hWnd, WM_QUIT, 0, 0);
						}
						break;
					}
				}
				else if (GetdJoyPovTrigger(POV_UP, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

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

					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0, (CONTROLLER)i) == true || dJoyStickTrigger(DIRESTICK_UP, STICK_LEFT, (CONTROLLER)i))
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック

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
					g_pVtxBuffVs->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffVs->Unlock();//プレイヤーバッファのアンロック
				}
			}
		}
	}

	if (g_VsState != VSSTATE_PAUSE)
	{
		UpdateBack();//背景
		UpdateBullet();//弾
		UpdatePlayer();//プレイヤー
		UpdatePlayer2();
		UpdateEffect();
		UpdateExef();
		UpdateExplosion();//爆発
	}
}

//-------------------
//描画処理
//-------------------
void DrawVs(void)
{

	DrawBack();//背景
	DrawBullet();//弾
	DrawPlayer();//プレイヤー
	DrawPlayer2();
	DrawEffect();
	DrawExef();
	DrawExplosion();//爆発

	if (g_VsState == VSSTATE_PAUSE)
	{
		LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

		//デバイスの取得
		pDevice = GetDevice();

		//頂点バッファ
		pDevice->SetStreamSource(0, g_pVtxBuffVs, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < SELECT_MAX; i++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureVs[i]);

			//背景の描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				i * VT_MAX,//始まりの番号
				2//ポリゴンの個数
			);
		}
	}
}

//----------------------
//ゲーム状態切替
//----------------------
void SetVsState(VSSTATE state)
{
	g_VsState = state;
	g_nCounterVsState = 0;
}

//-----------------------
//ゲーム状態取得
//-----------------------
VSSTATE GetVsState(void)
{
	return g_VsState;
}