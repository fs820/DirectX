//---------------------------------------
//
//背景表示処理[back.cpp]
//Author fuma sato
//
//---------------------------------------

#include"main.h"
#include"back.h"

#define NUM_BG (3)
#define BACK_SPEED (0.05f)

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureBack[NUM_BG] = { NULL };//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBack = NULL;//バッファのポインタ
float g_aPosTexU[NUM_BG];
int g_nCounterAnimBack;//更新間隔管理
int g_nPatternAnimBack;//テクスチャパターン管理

//----------------------
//背景の初期化処理
//----------------------
void InitBack(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//バッファの作成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBack,
		NULL
	);

	//1つ目

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BG1,
		&g_apTextureBack[0]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BG2,
		&g_apTextureBack[1]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BG3,
		&g_apTextureBack[2]
	);

	for (int i = 0; i < NUM_BG; i++)
	{
		g_aPosTexU[i] = 0.0f;
	}

	g_nCounterAnimBack = 0;
	g_nPatternAnimBack = 0;

	g_pVtxBuffBack->Lock(0, 0, (void**)&pVtx, 0);//バッファのロック

	for (int i = 0; i < NUM_BG; i++)
	{
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
		pVtx[0].tex = D3DXVECTOR2(g_aPosTexU[i], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aPosTexU[i] + 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aPosTexU[i], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aPosTexU[i] + 1.0f, 1.0f);

		pVtx += VT_MAX;
	}

	g_pVtxBuffBack->Unlock();//バッファのアンロック
}

//-------------------
//背景終了処理
//-------------------
void UninitBack(void)
{
	for (int i = 0; i < NUM_BG; i++)
	{
		//テクスチャの破棄
		if (g_apTextureBack[i] != NULL)
		{
			g_apTextureBack[i]->Release();
			g_apTextureBack[i] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBack != NULL)
	{
		g_pVtxBuffBack->Release();
		g_pVtxBuffBack = NULL;
	}
}

//-------------------
//背景更新処理
//-------------------
void UpdateBack(void)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ

	g_pVtxBuffBack->Lock(0, 0, (void**)&pVtx, 0);//バッファのロック

	for (int i = 0; i < NUM_BG; i++)
	{
		g_aPosTexU[i] += BACK_SPEED * ((i + 0.1f) * 0.1f);

		//テクスチャ
		pVtx[0].tex = D3DXVECTOR2(g_aPosTexU[i], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aPosTexU[i] + 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aPosTexU[i], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aPosTexU[i] + 1.0f, 1.0f);

		pVtx += VT_MAX;
	}

	g_pVtxBuffBack->Unlock();//バッファのアンロック
}

//-------------------
//背景描画処理
//-------------------
void DrawBack(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffBack, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < NUM_BG; i++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureBack[i]);

		//背景の描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//タイプ
			VT_MAX * i,//始まりの番号
			2//ポリゴンの個数
		);
	}
}