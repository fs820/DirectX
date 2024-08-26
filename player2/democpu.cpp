//---------------------------------------
//
//デモプレイコンピューター表示処理[democpu.cpp]
//Author fuma sato
//
//---------------------------------------

#include"democpu.h"
#include"back.h"
#include"bullet.h"
#include"explosion.h"
#include"score.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureDemoCpu;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDemoCpu = NULL;//バッファのポインタ
DEMOCPU g_DemoCpu;//プレイヤー情報
D3DXVECTOR3 g_moveBulletCpu;//弾発射向き・スピード管理

//----------------------
//プレイヤーの初期化処理
//----------------------
void InitDemoCpu(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDemoCpu,
		NULL
	);

	g_DemoCpu.nCounterAnim = 0;
	g_DemoCpu.nPatternAnim = 0;
	g_DemoCpu.pos = D3DXVECTOR3(DEMOCPU_WIDTH, SCREEN_HEIGHT / 2, 0);//初期位置
	g_DemoCpu.move = D3DXVECTOR3(DEMOCPU_SPEED_DEF, DEMOCPU_SPEED_DEF, DEMOCPU_SPEED_DEF);//プレイヤースピード
	g_DemoCpu.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
	g_DemoCpu.rotDef = g_DemoCpu.rot;//初期保存
	g_DemoCpu.fLength = sqrtf(DEMOCPU_WIDTH * DEMOCPU_WIDTH + DEMOCPU_HEIGHT * DEMOCPU_HEIGHT) / 2.0f;//対角線の長さ
	g_DemoCpu.fLengthDef = g_DemoCpu.fLength;//初期保存
	g_DemoCpu.fAngle = atan2f(DEMOCPU_WIDTH, DEMOCPU_HEIGHT);//角度
	g_DemoCpu.nLife = 100;
	g_DemoCpu.state = DEMOCPUSTATE_NORMAL;
	g_DemoCpu.nCounterState = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME2,
		&g_pTextureDemoCpu
	);

	g_pVtxBuffDemoCpu->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	//座標設定
	pVtx[0].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z - (D3DX_PI - g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
	pVtx[0].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z - (D3DX_PI - g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z - (D3DX_PI + g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
	pVtx[1].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z - (D3DX_PI + g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z - g_DemoCpu.fAngle) * g_DemoCpu.fLength;
	pVtx[2].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z - g_DemoCpu.fAngle) * g_DemoCpu.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z + g_DemoCpu.fAngle) * g_DemoCpu.fLength;
	pVtx[3].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z + g_DemoCpu.fAngle) * g_DemoCpu.fLength;
	pVtx[3].pos.z = 0.0f;

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
	pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_C, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX_C);
	pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_C, UV_DEF / V_MAX_C);

	g_pVtxBuffDemoCpu->Unlock();//プレイヤーバッファのアンロック
}

//-------------------
//プレイヤー終了処理
//-------------------
void UninitDemoCpu(void)
{
	//テクスチャの破棄
	if (g_pTextureDemoCpu != NULL)
	{
		g_pTextureDemoCpu->Release();
		g_pTextureDemoCpu = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffDemoCpu != NULL)
	{
		g_pVtxBuffDemoCpu->Release();
		g_pVtxBuffDemoCpu = NULL;
	}
}

//-------------------
//プレイヤー更新処理
//-------------------
void UpdateDemoCpu(void)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ
	int nRand = 0;
	nRand = rand() % 100;

	//移動処理
	if (nRand >= 0 && nRand < 30)//Aキー
	{
		if (nRand >= 0 && nRand < 10)//Wキー
		{//左上
			g_DemoCpu.move.x += sinf(D3DX_PI * -0.75f) * DEMOCPU_SPEED_X;
			g_DemoCpu.move.y += cosf(D3DX_PI * -0.75f) * DEMOCPU_SPEED_Y;
		}
		else if (nRand >= 10 && nRand < 20)//Sキー
		{//左下
			g_DemoCpu.move.x += sinf(D3DX_PI * -0.25f) * DEMOCPU_SPEED_X;
			g_DemoCpu.move.y += cosf(D3DX_PI * -0.25f) * DEMOCPU_SPEED_Y;
		}
		else
		{//左
			g_DemoCpu.move.x += -DEMOCPU_SPEED_X;
		}
	}
	else if (nRand >= 30 && nRand < 60)//Dキー
	{
		if (nRand >= 30 && nRand < 40)//Wキー
		{//右上
			g_DemoCpu.move.x += sinf(D3DX_PI * 0.75f) * DEMOCPU_SPEED_X;
			g_DemoCpu.move.y += cosf(D3DX_PI * 0.75f) * DEMOCPU_SPEED_Y;
		}
		else if (nRand >= 40 && nRand < 50)//Sキー
		{//右下
			g_DemoCpu.move.x += sinf(D3DX_PI * 0.25f) * DEMOCPU_SPEED_X;
			g_DemoCpu.move.y += cosf(D3DX_PI * 0.25f) * DEMOCPU_SPEED_Y;
		}
		else
		{//右
			g_DemoCpu.move.x += DEMOCPU_SPEED_X;
		}
	}
	else if (nRand >= 60 && nRand < 70)//Wキー
	{//上
		g_DemoCpu.move.y += -DEMOCPU_SPEED_Y;
	}
	else if (nRand >= 70 && nRand < 80)//Sキー
	{//下
		g_DemoCpu.move.y += DEMOCPU_SPEED_Y;
	}

	if (nRand >= 80 && nRand < 82)
	{
		g_DemoCpu.rot.z += DEMOCPU_ROT_SPEED;
	}
	if (nRand >= 82 && nRand < 84)
	{
		g_DemoCpu.rot.z += -DEMOCPU_ROT_SPEED;
	}
	if (nRand >= 84 && nRand < 86)
	{
		g_DemoCpu.rot = g_DemoCpu.rotDef;
	}

	if (nRand >= 86 && nRand < 88)
	{
		if (g_DemoCpu.fLength >= 0.0f && g_DemoCpu.fLength <= DEMOCPU_SIZE_MAX)
		{
			g_DemoCpu.fLength += DEMOCPU_SIZE_SPEED;
		}
	}
	if (nRand >= 88 && nRand < 90)
	{
		if (g_DemoCpu.fLength >= DEMOCPU_SIZE_MIN)
		{
			g_DemoCpu.fLength += -DEMOCPU_SIZE_SPEED;
		}
	}
	if (nRand >= 90 && nRand < 92)
	{
		g_DemoCpu.fLength = g_DemoCpu.fLengthDef;
	}

	if (nRand >= 92 && nRand < 94)
	{
		g_moveBulletCpu.x = sinf(g_DemoCpu.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;
		g_moveBulletCpu.y = cosf(g_DemoCpu.rot.z + D3DX_PI * 0.5f) * BULLET_SPEED;

		//弾の設定
		SetBullet(g_DemoCpu.pos, g_moveBulletCpu, g_DemoCpu.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);
	}
	if (nRand >= 94 && nRand < 96)
	{
		//弾の設定
		Set3Bullet(g_DemoCpu.pos, g_DemoCpu.rot.z, g_DemoCpu.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, 1);
	}
	if (nRand >= 96 && nRand < 98)
	{
		//弾の設定
		SetAllBullet(g_DemoCpu.pos, g_DemoCpu.rot.z, g_DemoCpu.fLength, BULLET_LIFE, BULLETTYPE_PLAYER, BULLET_INTER);
	}

	if (nRand >= 98 && nRand < 100)
	{
		SetExplosion(g_DemoCpu.pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_DemoCpu.fLength);
		g_DemoCpu.nLife = 0;
		g_DemoCpu.state = DEMOCPUSTATE_DIE;
	}

	//位置の更新
	g_DemoCpu.pos.x += g_DemoCpu.move.x;
	g_DemoCpu.pos.y += g_DemoCpu.move.y;

	//移動量の更新(減衰)
	g_DemoCpu.move.x += (DEMOCPU_SPEED_DEF - g_DemoCpu.move.x) * DEMOCPU_INA;
	g_DemoCpu.move.y += (DEMOCPU_SPEED_DEF - g_DemoCpu.move.y) * DEMOCPU_INA;

	g_DemoCpu.nCounterAnim++;
	if ((g_DemoCpu.nCounterAnim % UP_TIME) == 0)//更新期間
	{
		g_DemoCpu.nCounterAnim = (g_DemoCpu.nPatternAnim + 1) % (U_MAX_C * V_MAX_C);

		g_pVtxBuffDemoCpu->Lock(0, 0, (void**)&pVtx, 0);//プレイヤバッファのロック

	//座標設定
		pVtx[0].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z - (D3DX_PI - g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
		pVtx[0].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z - (D3DX_PI - g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z - (D3DX_PI + g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
		pVtx[1].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z - (D3DX_PI + g_DemoCpu.fAngle)) * g_DemoCpu.fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z - g_DemoCpu.fAngle) * g_DemoCpu.fLength;
		pVtx[2].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z - g_DemoCpu.fAngle) * g_DemoCpu.fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = g_DemoCpu.pos.x + sinf(g_DemoCpu.rot.z + g_DemoCpu.fAngle) * g_DemoCpu.fLength;
		pVtx[3].pos.y = g_DemoCpu.pos.y + cosf(g_DemoCpu.rot.z + g_DemoCpu.fAngle) * g_DemoCpu.fLength;
		pVtx[3].pos.z = 0.0f;

		//テクスチャ
		pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_C * g_DemoCpu.nPatternAnim, UV_DEF / V_MAX_C * (g_DemoCpu.nPatternAnim / U_MAX_C));
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_C * g_DemoCpu.nPatternAnim + UV_DEF / U_MAX_C, UV_DEF / V_MAX_C * (g_DemoCpu.nPatternAnim / U_MAX_C));
		pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_C * g_DemoCpu.nPatternAnim, UV_DEF / V_MAX_C * (g_DemoCpu.nPatternAnim / U_MAX_C) + UV_DEF / V_MAX_C);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_C * g_DemoCpu.nPatternAnim + UV_DEF / U_MAX_C, UV_DEF / V_MAX_C * (g_DemoCpu.nPatternAnim / U_MAX_C) + UV_DEF / V_MAX_C);

		if (pVtx[0].pos.x >= SCREEN_WIDTH || pVtx[1].pos.x >= SCREEN_WIDTH || pVtx[2].pos.x >= SCREEN_WIDTH || pVtx[3].pos.x >= SCREEN_WIDTH)//画面右端
		{
			g_DemoCpu.pos.x = g_DemoCpu.fLength;//画面左端
		}
		else if (pVtx[0].pos.x <= 0 || pVtx[1].pos.x <= 0 || pVtx[2].pos.x <= 0 || pVtx[3].pos.x <= 0)//画面左端
		{
			g_DemoCpu.pos.x = SCREEN_WIDTH - g_DemoCpu.fLength;//画面右端
		}

		if (pVtx[0].pos.y >= SCREEN_HEIGHT || pVtx[1].pos.y >= SCREEN_HEIGHT || pVtx[2].pos.y >= SCREEN_HEIGHT || pVtx[3].pos.y >= SCREEN_HEIGHT)//画面下端
		{
			g_DemoCpu.pos.y = g_DemoCpu.fLength;//画面上端
		}
		else if (pVtx[0].pos.y <= 0 || pVtx[1].pos.y <= 0 || pVtx[2].pos.y <= 0 || pVtx[3].pos.y <= 0)
		{
			g_DemoCpu.pos.y = SCREEN_HEIGHT - g_DemoCpu.fLength;//画面下端
		}

		switch (g_DemoCpu.state)
		{
		case DEMOCPUSTATE_DAMAGE:
			g_DemoCpu.nCounterState--;
			if (g_DemoCpu.nCounterState <= 0)
			{
				g_DemoCpu.state = DEMOCPUSTATE_NORMAL;

				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			break;
		}

		g_pVtxBuffDemoCpu->Unlock();//プレイヤーバッファのアンロック
	}
}

//-------------------
//プレイヤー描画処理
//-------------------
void DrawDemoCpu(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffDemoCpu, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureDemoCpu);

	//プレイヤーの描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//タイプ
		0,//始まりの番号
		2//ポリゴンの個数
	);
}

//-------------------
//ヒット処理
//-------------------
void HitDemoCpu(int nDamage)
{
	VERTEX_2D* pVtx;

	g_DemoCpu.nLife -= nDamage;

	if (g_DemoCpu.nLife <= 0)
	{
		SetExplosion(g_DemoCpu.pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_DemoCpu.fLength);
		g_DemoCpu.state = DEMOCPUSTATE_DIE;
		AddScore(-nDamage * SCORE_DIE);
	}
	else
	{
		g_DemoCpu.state = DEMOCPUSTATE_DAMAGE;
		g_DemoCpu.nCounterState = 5;
		AddScore(-nDamage * SCORE_MISS);

		g_pVtxBuffDemoCpu->Lock(0, 0, (void**)&pVtx, 0);
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		g_pVtxBuffDemoCpu->Unlock();//バッファのアンロック
	}
}

//-------------------------
//プレイヤーバッファの取得
//-------------------------
PDIRECT3DVERTEXBUFFER9 GetDemoCpuBuffer(void)
{
	return g_pVtxBuffDemoCpu;
}

//-------------------------
//プレイヤー取得
//-------------------------
DEMOCPU* GetDemoCpu(void)
{
	return &g_DemoCpu;
}