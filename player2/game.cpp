//------------------------------------------
//
//ゲームメインの処理[game.cpp]
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

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureGame[SELECT_MAX] = { NULL };//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;//バッファのポインタ
GAMESTATE g_gameState = GAMESTATE_NONE;
int g_nCounterGameState = 0;

//--------------------
//初期化処理
//--------------------
void InitGame(void)
{
	InitBack();//背景
	InitBullet();//弾
	InitPlayer();//プレイヤー
	InitEnemy();//敵
	InitEffect();
	InitExplosion();//爆発
	InitScore();//スコア
	SetScore(0);//スコア書き換え
	SetEnemy(D3DXVECTOR3(1000.0f, 200.0f, 0.0f), 0);//敵発生
	SetEnemy(D3DXVECTOR3(1000.0f, 500.0f, 0.0f), 1);//敵発生
	SetEnemy(D3DXVECTOR3(1000.0f, 350.0f, 0.0f), 2);//敵発生
	SetEnemy(D3DXVECTOR3(700.0f, 350.0f, 0.0f), 3);//敵発生

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
		&g_pVtxBuffGame,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTRESTART,
		&g_apTextureGame[0]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTRETRY,
		&g_apTextureGame[1]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTBACK,
		&g_apTextureGame[2]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SELECTEXIT,
		&g_apTextureGame[3]
	);


	g_gameState = GAMESTATE_NORMAL;
	g_nCounterGameState = 0;
	posSelect = D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2-SELECT_HEIGHT*1.5f,0.0f);

	g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

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

	g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

	PlaySound(SOUND_LABEL_BGM4);
}

//------------------
//終了処理
//------------------
void UninitGame(void)
{
	g_gameState = GAMESTATE_NONE;

	//サウンド
	StopSound();

	UninitScore();//スコア
	UninitExplosion();//爆発
	UninitEffect();
	UninitEnemy();//敵
	UninitPlayer();//プレイヤー
	UninitBullet();//弾
	UninitBack();//背景
}

//--------------
//更新処理
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
				VERTEX_2D* pVtx;//頂点情報ポインタ

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * SelectNew;

				//カラー
				pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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
					//サウンド
					StopSound();
					//切替
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
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_GAME);
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

			g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

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
							//サウンド
							StopSound();
							//切替
							SetFade(MODE_GAME);
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

			g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
		}
		else if (bStick==false)
		{
			if (GetKeyboradTrigger(DIK_UP) == true || GetKeyboradTrigger(DIK_W) == true || GetJoykeyTrigger(JOYKEY_UP) == true || *(GetJoyStick(STICK_LEFT) + 1) > STICK_DED || GetMouseWheel() > 0.0f)
			{
				VERTEX_2D* pVtx;//頂点情報ポインタ

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * SelectNew;

				//カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * SelectNew;

				//カラー
				pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
			}
			else if (GetKeyboradTrigger(DIK_DOWN) == true || GetKeyboradTrigger(DIK_S) == true || GetJoykeyTrigger(JOYKEY_DOWN) == true || *(GetJoyStick(STICK_LEFT) + 1) < -STICK_DED || GetMouseWheel() < 0.0f)
			{
				VERTEX_2D* pVtx;//頂点情報ポインタ

				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * SelectNew;

				//カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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
				g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

				pVtx += VT_MAX * SelectNew;

				//カラー
				pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
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
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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
						//サウンド
						StopSound();
						//切替
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
						//サウンド
						StopSound();
						//切替
						SetFade(MODE_GAME);
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
			else if (bStick == false)
			{
				if (GetdJoyPovTrigger(POV_UP, 0) == true || *(GetdJoyStick(STICK_LEFT) + 1) < -STICK_DED)
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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

					g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
				}
				else if (GetdJoyPovTrigger(POV_DOWN, 0) == true || *(GetdJoyStick(STICK_LEFT) + 1) > STICK_DED)
				{
					VERTEX_2D* pVtx;//頂点情報ポインタ

					g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック

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
					g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

					pVtx += VT_MAX * SelectNew;

					//カラー
					pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

					g_pVtxBuffGame->Unlock();//プレイヤーバッファのアンロック
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
		UpdateBack();//背景
		UpdateBullet();//弾
		UpdatePlayer();//プレイヤー
		UpdateEnemy();//敵
		UpdateEffect();
		UpdateExplosion();//爆発
		UpdateScore();//スコア
	}
}

//-------------------
//描画処理
//-------------------
void DrawGame(void)
{

	DrawBack();//背景
	DrawBullet();//弾
	DrawPlayer();//プレイヤー
	DrawEnemy();//敵
	DrawEffect();
	DrawExplosion();//爆発
	DrawScore();//スコア

	if(g_gameState==GAMESTATE_PAUSE)
	{
		LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

        //デバイスの取得
		pDevice = GetDevice();

		//頂点バッファ
		pDevice->SetStreamSource(0, g_pVtxBuffGame, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < SELECT_MAX; i++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureGame[i]);

			//背景の描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				i*VT_MAX,//始まりの番号
				2//ポリゴンの個数
			);
		}
	}
}

//----------------------
//ゲーム状態切替
//----------------------
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//-----------------------
//ゲーム状態取得
//-----------------------
GAMESTATE GetGameState(void)
{
	return g_gameState;
}