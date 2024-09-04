//---------------------------------------
//
//背景表示処理[p4back.cpp]
//Author fuma sato
//
//---------------------------------------

#include"main.h"
#include"p4back.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureP4Back = NULL;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffP4Back = NULL;//バッファのポインタ
float g_PosTexUP4;
float g_PosTexVP4;
int g_nCounterAnimP4Back;//更新間隔管理
int g_nPatternAnimP4Back;//テクスチャパターン管理

//----------------------
//背景の初期化処理
//----------------------
void InitP4Back(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//バッファの作成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffP4Back,
		NULL
	);

	//1つ目

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_FIELD,
		&g_pTextureP4Back
	);

	g_PosTexUP4 = 0.0f;
	g_PosTexVP4 = 0.0f;

	g_nCounterAnimP4Back = 0;
	g_nPatternAnimP4Back = 0;

	g_pVtxBuffP4Back->Lock(0, 0, (void**)&pVtx, 0);//バッファのロック

		//座標設定
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - SCREEN_WIDTH / 4, SCREEN_HEIGHT - SCREEN_HEIGHT / 4, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - SCREEN_HEIGHT / 4, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - SCREEN_WIDTH / 4, SCREEN_HEIGHT, 0.0f);
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
	pVtx[0].tex = D3DXVECTOR2(g_PosTexUP4, g_PosTexVP4);
	pVtx[1].tex = D3DXVECTOR2(g_PosTexUP4 + 1.0f, g_PosTexVP4);
	pVtx[2].tex = D3DXVECTOR2(g_PosTexUP4, g_PosTexVP4 + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_PosTexUP4 + 1.0f, g_PosTexVP4 + 1.0f);

	g_pVtxBuffP4Back->Unlock();//バッファのアンロック
}

//-------------------
//背景終了処理
//-------------------
void UninitP4Back(void)
{
	//テクスチャの破棄
	if (g_pTextureP4Back != NULL)
	{
		g_pTextureP4Back->Release();
		g_pTextureP4Back = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffP4Back != NULL)
	{
		g_pVtxBuffP4Back->Release();
		g_pVtxBuffP4Back = NULL;
	}
}

//-------------------
//背景更新処理
//-------------------
void UpdateP4Back(void)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ

	g_pVtxBuffP4Back->Lock(0, 0, (void**)&pVtx, 0);//バッファのロック

	int nRand = 0;
	nRand = rand() % 4;
	switch (nRand)
	{
	case 0:
		g_PosTexUP4 += ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		g_PosTexVP4 += ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		break;
	case 1:
		g_PosTexUP4 += ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		g_PosTexVP4 -= ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		break;
	case 2:
		g_PosTexUP4 -= ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		g_PosTexVP4 += ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		break;
	case 3:
		g_PosTexUP4 -= ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		g_PosTexVP4 -= ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		break;
	}

	//テクスチャ
	pVtx[0].tex = D3DXVECTOR2(g_PosTexUP4, g_PosTexVP4);
	pVtx[1].tex = D3DXVECTOR2(g_PosTexUP4 + 1.0f, g_PosTexVP4);
	pVtx[2].tex = D3DXVECTOR2(g_PosTexUP4, g_PosTexVP4 + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_PosTexUP4 + 1.0f, g_PosTexVP4 + 1.0f);

	g_pVtxBuffP4Back->Unlock();//バッファのアンロック
}

//-------------------
//背景描画処理
//-------------------
void DrawP4Back(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffP4Back, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureP4Back);

	//背景の描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//タイプ
		0,//始まりの番号
		2//ポリゴンの個数
	);
}