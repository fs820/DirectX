//---------------------------------------
//
//弾表示処理[explosion.cpp]
//Author fuma sato
//
//---------------------------------------

#include"explosion.h"
#include"sound.h"

#define MAX_EXPLOSION (256)
#define EXPLOSION_WIDTH (25.0f)
#define EXPLOSION_HEIGHT (25.0f)

//弾構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXCOLOR col;
	int nCntAnim;//移動量
	int nPatternAnim;//寿命
	float fLength;
	bool bUse;//使用しているかどうか
}Explosion;

//グローバル
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;//バッファのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];//弾の情報

//-----------------
//弾の初期化処理
//-----------------
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//デバイスを取得
	pDevice = GetDevice();

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		g_aExplosion[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aExplosion[i].nCntAnim = 0;
		g_aExplosion[i].nPatternAnim = 0;
		g_aExplosion[i].fLength = 0.0f;
		g_aExplosion[i].bUse = false;//使用していない状態にする
	}

	//バッファの作成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_EXP,
		&g_pTextureExplosion
	);

	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH / 2 * g_aExplosion[i].fLength, g_aExplosion[i].pos.y - EXPLOSION_HEIGHT / 2 * g_aExplosion[i].fLength, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH / 2 * g_aExplosion[i].fLength, g_aExplosion[i].pos.y - EXPLOSION_HEIGHT / 2 * g_aExplosion[i].fLength, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH / 2 * g_aExplosion[i].fLength, g_aExplosion[i].pos.y + EXPLOSION_HEIGHT / 2 * g_aExplosion[i].fLength, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH / 2 * g_aExplosion[i].fLength, g_aExplosion[i].pos.y + EXPLOSION_HEIGHT / 2 * g_aExplosion[i].fLength, 0.0f);

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
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y - EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y - EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y + EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y + EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);

		pVtx += VT_MAX;//ポリゴン1つ分進める
	}
	g_pVtxBuffExplosion->Unlock();//バッファのアンロック
}

//----------------
//弾の終了処理
//----------------
void UninitExplosion(void)
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//----------------
//弾の更新処理
//----------------
void UpdateExplosion(void)
{
	VERTEX_2D* pVtx;

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (g_aExplosion[i].bUse == true)
		{//弾が使用されている

			g_aExplosion[i].nCntAnim++;
			if ((g_aExplosion[i].nCntAnim % UP_TIME) == 0)//更新期間
			{
				g_aExplosion[i].nCntAnim = (g_aExplosion[i].nCntAnim + 1) % (U_MAX_E * V_MAX_E);

				g_aExplosion[i].nPatternAnim++;
				if (g_aExplosion[i].nPatternAnim >= (U_MAX_E * V_MAX_E))
				{
					g_aExplosion[i].bUse = false;
				}

				g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);//バッファのロック
				pVtx += i * VT_MAX;

				//テクスチャ
				pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E));
				pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim + UV_DEF / U_MAX_E, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E));
				pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E) + UV_DEF / V_MAX_E);
				pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_E * g_aExplosion[i].nPatternAnim + UV_DEF / U_MAX_E, UV_DEF / V_MAX_E * (g_aExplosion[i].nPatternAnim / U_MAX_E) + UV_DEF / V_MAX_E);

				g_pVtxBuffExplosion->Unlock();//バッファのアンロック
			}
		}
	}
}

//----------------
//弾の描画処理
//----------------
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (g_aExplosion[i].bUse == true)
		{//弾が使用されている
			//弾の描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				i * VT_MAX,//始まりの番号
				2//ポリゴンの個数
			);
		}
	}
}

//-------------------
//爆発設定
//-------------------
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, float fLengthBullet)
{

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		if (g_aExplosion[i].bUse == false)
		{//弾が使用されていない
			g_aExplosion[i].pos = pos;
			g_aExplosion[i].fLength = fLengthBullet;
			g_aExplosion[i].nCntAnim = 0;
			g_aExplosion[i].nPatternAnim = 0;
			g_aExplosion[i].col = col;
			g_aExplosion[i].bUse = true;

			//頂点座標設定
			VERTEX_2D* pVtx;

			g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += i * VT_MAX;//ポリゴン1つ分進める
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y - EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y - EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[i].pos.x - EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y + EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[i].pos.x + EXPLOSION_WIDTH / 2 * (g_aExplosion[i].fLength * 0.2f), g_aExplosion[i].pos.y + EXPLOSION_HEIGHT / 2 * (g_aExplosion[i].fLength * 0.2f), 0.0f);

			g_pVtxBuffExplosion->Unlock();//バッファのアンロック

		    PlaySound(SOUND_LABEL_EXPLOSION);
			break;
		}
	}
}