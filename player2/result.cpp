//------------------------------------------
//
//リザルトの処理[result.cpp]
//Author fuma sato
//
//------------------------------------------

#include"result.h"
#include"input.h"
#include"fade.h"
#include"sound.h"
#include"score.h"
#include"data.h"

#define RESULT_TIME (600)

LPDIRECT3DTEXTURE9 g_apTextureResult[RESULT_MAX] = { NULL };//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;//バッファのポインタ

//--------------------
//初期化処理
//--------------------
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ
	D3DXVECTOR3 posScore;//スコアの位置
	int nScore = 0;
	int aPosTexUr[SCORE_MAX];

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX*(SCORE_MAX+1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_RESULT,
		&g_apTextureResult[0]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SCORE,
		&g_apTextureResult[1]
	);

	//スコアの取得
	nScore = GetScore();
	for (int i = 0; i < SCORE_MAX; i++)
	{
		aPosTexUr[i] = Digit(nScore,i);
	}
	posScore = D3DXVECTOR3(SCREEN_WIDTH/2-(SCORE_WIDTH/U_MAX_S)*(SCORE_MAX/2), SCREEN_HEIGHT/2, 0.0f);

	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

		//座標設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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

	for (int i = 0; i < SCORE_MAX; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(posScore.x - (SCORE_WIDTH / U_MAX_S) / 2, posScore.y - (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);
		pVtx[1].pos = D3DXVECTOR3(posScore.x + (SCORE_WIDTH / U_MAX_S) / 2, posScore.y - (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);
		pVtx[2].pos = D3DXVECTOR3(posScore.x - (SCORE_WIDTH / U_MAX_S) / 2, posScore.y + (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);
		pVtx[3].pos = D3DXVECTOR3(posScore.x + (SCORE_WIDTH / U_MAX_S) / 2, posScore.y + (SCORE_HEIGHT / V_MAX_S) / 2, posScore.z);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//テクスチャ
		pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i], UV_DEF / V_MAX_S * (aPosTexUr[i] / U_MAX_S));
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexUr[i] / U_MAX_S));
		pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i], UV_DEF / V_MAX_S * (aPosTexUr[i] / U_MAX_S) + UV_DEF / V_MAX_S);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexUr[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexUr[i] / U_MAX_S) + UV_DEF / V_MAX_S);

		posScore.x += SCORE_WIDTH / U_MAX_S;
		pVtx += VT_MAX;
	}

	g_pVtxBuffResult->Unlock();//プレイヤーバッファのアンロック
	PlaySound(SOUND_LABEL_BGM5);
}

//------------------
//終了処理
//------------------
void UninitResult(void)
{
	//サウンド
	StopSound();
	//テクスチャの破棄
	for (int i = 0; i < RESULT_MAX; i++)
	{
		if (g_apTextureResult[i] != NULL)
		{
			g_apTextureResult[i]->Release();
			g_apTextureResult[i] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//--------------
//更新処理
//--------------
void UpdateResult(void)
{
	static int nResultTime = 0;

	if (GetKeyboradTrigger(DIK_RETURN) == true || GetJoykeyTrigger(JOYKEY_START,CONTROLLER_MAX) == true||GetMouseTrigger(MOUSE_LEFT)==true)
	{
		nResultTime = 0;
		FADE fade;
		fade = GetFade();
		if (fade == FADE_NONE)
		{
			//サウンド
			StopSound();
			//切替
			SetFade(MODE_RANK);
		}
	}

	for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
	{
		if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
		{
			if (GetdJoykeyTrigger(ELEKEY_START, (CONTROLLER)i))
			{
				nResultTime = 0;
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_RANK);
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
		{
			if (GetdJoykeyTrigger(PSKEY_START, (CONTROLLER)i))
			{
				nResultTime = 0;
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_RANK);
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
		{
			if (GetdJoykeyTrigger(NINKEY_＋, (CONTROLLER)i))
			{
				nResultTime = 0;
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_RANK);
				}
			}
		}
		else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
		{
			if (GetdJoykeyTrigger(DKEY_START, (CONTROLLER)i))
			{
				nResultTime = 0;
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_RANK);
				}
			}
		}
	}

	if (nResultTime%RESULT_TIME==0)
	{
		FADE fade;
		fade = GetFade();
		if (fade == FADE_NONE)
		{
			//サウンド
			StopSound();
			//切替
			SetFade(MODE_RANK);
		}
	}

	nResultTime++;
}

//-------------------
//描画処理
//-------------------
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureResult[0]);

	//背景の描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//タイプ
		0,//始まりの番号
		2//ポリゴンの個数
	);

	for (int i = 0; i < SCORE_MAX; i++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureResult[1]);

		//背景の描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//タイプ
			(i+1)*VT_MAX,//始まりの番号
			2//ポリゴンの個数
		);
	}
}