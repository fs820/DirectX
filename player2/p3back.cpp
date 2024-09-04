//---------------------------------------
//
//背景表示処理[p3back.cpp]
//Author fuma sato
//
//---------------------------------------

#include"main.h"
#include"p3back.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureP3Back = NULL;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffP3Back = NULL;//バッファのポインタ
float g_PosTexUP3;
float g_PosTexVP3;
int g_nCounterAnimP3Back;//更新間隔管理
int g_nPatternAnimP3Back;//テクスチャパターン管理

//----------------------
//背景の初期化処理
//----------------------
void InitP3Back(void)
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
		&g_pVtxBuffP3Back,
		NULL
	);

	//1つ目

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_FIELD,
		&g_pTextureP3Back
	);

	g_PosTexUP3 = 0.0f;
	g_PosTexVP3 = 0.0f;

	g_nCounterAnimP3Back = 0;
	g_nPatternAnimP3Back = 0;

	g_pVtxBuffP3Back->Lock(0, 0, (void**)&pVtx, 0);//バッファのロック

		//座標設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT-SCREEN_HEIGHT/4, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH/4, SCREEN_HEIGHT - SCREEN_HEIGHT / 4, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH/4, SCREEN_HEIGHT, 0.0f);

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
	pVtx[0].tex = D3DXVECTOR2(g_PosTexUP3, g_PosTexVP3);
	pVtx[1].tex = D3DXVECTOR2(g_PosTexUP3 + 1.0f, g_PosTexVP3);
	pVtx[2].tex = D3DXVECTOR2(g_PosTexUP3, g_PosTexVP3 + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_PosTexUP3 + 1.0f, g_PosTexVP3 + 1.0f);

	g_pVtxBuffP3Back->Unlock();//バッファのアンロック
}

//-------------------
//背景終了処理
//-------------------
void UninitP3Back(void)
{
	//テクスチャの破棄
	if (g_pTextureP3Back != NULL)
	{
		g_pTextureP3Back->Release();
		g_pTextureP3Back = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffP3Back != NULL)
	{
		g_pVtxBuffP3Back->Release();
		g_pVtxBuffP3Back = NULL;
	}
}

//-------------------
//背景更新処理
//-------------------
void UpdateP3Back(void)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ

	g_pVtxBuffP3Back->Lock(0, 0, (void**)&pVtx, 0);//バッファのロック

	int nRand = 0;
	nRand = rand() % 4;
	switch (nRand)
	{
	case 0:
		g_PosTexUP3 += ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		g_PosTexVP3 += ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		break;
	case 1:
		g_PosTexUP3 += ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		g_PosTexVP3 -= ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		break;
	case 2:
		g_PosTexUP3 -= ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		g_PosTexVP3 += ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		break;
	case 3:
		g_PosTexUP3 -= ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		g_PosTexVP3 -= ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		break;
	}

	//テクスチャ
	pVtx[0].tex = D3DXVECTOR2(g_PosTexUP3, g_PosTexVP3);
	pVtx[1].tex = D3DXVECTOR2(g_PosTexUP3 + 1.0f, g_PosTexVP3);
	pVtx[2].tex = D3DXVECTOR2(g_PosTexUP3, g_PosTexVP3 + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_PosTexUP3 + 1.0f, g_PosTexVP3 + 1.0f);

	g_pVtxBuffP3Back->Unlock();//バッファのアンロック
}

//-------------------
//背景描画処理
//-------------------
void DrawP3Back(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffP3Back, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureP3Back);

	//背景の描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//タイプ
		0,//始まりの番号
		2//ポリゴンの個数
	);
}