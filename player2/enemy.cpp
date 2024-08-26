//----------------------------------------
//
//敵表示処理[enemy.cpp]
//Author fuma sato
//
//----------------------------------------
#include"enemy.h"
#include"bullet.h"
#include"explosion.h"
#include"score.h"

//マクロ定義
#define NUM_ENEMY (4)
#define ENEMY_BULLET_INTER (10)
#define ENEMY_BULLET_SPEEDX (-10.0f)
#define ENEMY_BULLET_SPEEDY (0.0f)
#define ENEMY_BULLET_SIZE (100.0f)

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;//バッファのポインタ
Enemy g_aEnemy[MAX_ENEMY];
int g_nNumEnemy = 0;

//-----------------------
//敵の初期化処理
//-----------------------
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX*MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_ENE,
		&g_apTextureEnemy[0]
	);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_ENE2,
		&g_apTextureEnemy[1]
	);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_ENE3,
		&g_apTextureEnemy[2]
	);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_ENE4,
		&g_apTextureEnemy[3]
	);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_aEnemy[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[i].state = ENEMYSTATE_NORMAL;
		g_aEnemy[i].nLife = 100;
		g_aEnemy[i].nCounterState = 0;
		g_aEnemy[i].fLength= sqrtf(ENEMY_WIDTH * ENEMY_WIDTH + ENEMY_HEIGHT * ENEMY_HEIGHT) / 2.0f;
		g_aEnemy[i].nType = 0;
		g_aEnemy[i].bUse = false;
	}
	g_nNumEnemy = 0;

	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//頂点座標設定
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - ENEMY_WIDTH / 2, g_aEnemy[i].pos.y - ENEMY_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + ENEMY_WIDTH / 2, g_aEnemy[i].pos.y - ENEMY_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - ENEMY_WIDTH / 2, g_aEnemy[i].pos.y + ENEMY_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + ENEMY_WIDTH / 2, g_aEnemy[i].pos.y + ENEMY_HEIGHT / 2, 0.0f);

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

	g_pVtxBuffEnemy->Unlock();//プレイヤーバッファのアンロック
}

//---------------------
//敵の終了処理
//---------------------
void UninitEnemy(void)
{
	for (int i = 0; i < NUM_ENEMY; i++)
	{
		//テクスチャの破棄
		if (g_apTextureEnemy[i] != NULL)
		{
			g_apTextureEnemy[i]->Release();
			g_apTextureEnemy[i] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//-----------------------
//敵の更新処理
//-----------------------
void UpdateEnemy(void)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ
	static int aEnemyBulletCnt[MAX_ENEMY] = { 0 };

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_aEnemy[i].bUse == true)
		{
			switch (g_aEnemy[i].state)
			{
			case ENEMYSTATE_NORMAL:
				aEnemyBulletCnt[i]++;
				if (aEnemyBulletCnt[i] >= ENEMY_BULLET_INTER)
				{
					aEnemyBulletCnt[i] = 0;
					//弾の設定
					int nRand=0;
					nRand = rand() % 3;
					switch (nRand)
					{
					case 0:
						SetBullet(g_aEnemy[i].pos, D3DXVECTOR3(ENEMY_BULLET_SPEEDX, ENEMY_BULLET_SPEEDY, 0.0f), ENEMY_BULLET_SIZE, BULLET_LIFE, BULLETTYPE_ENEMY, BULLET_INTER);
						break;
					case 1:
						Set3Bullet(g_aEnemy[i].pos, D3DX_PI, ENEMY_BULLET_SIZE, BULLET_LIFE, BULLETTYPE_ENEMY, BULLET_INTER);
						break;
					case 2:
						SetAllBullet(g_aEnemy[i].pos, D3DX_PI, ENEMY_BULLET_SIZE, BULLET_LIFE, BULLETTYPE_ENEMY, BULLET_INTER);
						break;
					}
				}
				break;
			case ENEMYSTATE_DAMAGE:
				g_aEnemy[i].nCounterState--;
				if (g_aEnemy[i].nCounterState <= 0)
				{
					g_aEnemy[i].state = ENEMYSTATE_NORMAL;

					g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
					pVtx += i * VT_MAX;
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					g_pVtxBuffEnemy->Unlock();//バッファのアンロック
				}
			}
		}
	}
}

//--------------------
//敵の描画処理
//--------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_aEnemy[i].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[i].nType]);

			//背景の描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				VT_MAX * i,//始まりの番号
				2//ポリゴンの個数
			);
		}
	}
}

//-------------------
//敵設定
//-------------------
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_aEnemy[i].bUse == false)
		{//弾が使用されていない
			g_aEnemy[i].pos = pos;
			g_aEnemy[i].nType = nType;
			g_aEnemy[i].state = ENEMYSTATE_NORMAL;
			g_aEnemy[i].nLife = 100;
			g_aEnemy[i].nCounterState = 0;
			g_aEnemy[i].bUse = true;

			//頂点座標設定

			VERTEX_2D* pVtx;

			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
			pVtx += i * VT_MAX;//ポリゴン1つ分進める

			switch (g_aEnemy[i].nType)
			{
			case 0:
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - ENEMY_WIDTH / 2, g_aEnemy[i].pos.y - ENEMY_HEIGHT / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + ENEMY_WIDTH / 2, g_aEnemy[i].pos.y - ENEMY_HEIGHT / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - ENEMY_WIDTH / 2, g_aEnemy[i].pos.y + ENEMY_HEIGHT / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + ENEMY_WIDTH / 2, g_aEnemy[i].pos.y + ENEMY_HEIGHT / 2, 0.0f);
				break;
			case 1:
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - ENEMY_WIDTH2 / 2, g_aEnemy[i].pos.y - ENEMY_HEIGHT2 / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + ENEMY_WIDTH2 / 2, g_aEnemy[i].pos.y - ENEMY_HEIGHT2 / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - ENEMY_WIDTH2 / 2, g_aEnemy[i].pos.y + ENEMY_HEIGHT2 / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + ENEMY_WIDTH2 / 2, g_aEnemy[i].pos.y + ENEMY_HEIGHT2 / 2, 0.0f);
				break;
			case 2:
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - ENEMY_WIDTH3 / 2, g_aEnemy[i].pos.y - ENEMY_HEIGHT3 / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + ENEMY_WIDTH3 / 2, g_aEnemy[i].pos.y - ENEMY_HEIGHT3 / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - ENEMY_WIDTH3 / 2, g_aEnemy[i].pos.y + ENEMY_HEIGHT3 / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + ENEMY_WIDTH3 / 2, g_aEnemy[i].pos.y + ENEMY_HEIGHT3 / 2, 0.0f);
				break;
			case 3:
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - ENEMY_WIDTH4 / 2, g_aEnemy[i].pos.y - ENEMY_HEIGHT4 / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + ENEMY_WIDTH4 / 2, g_aEnemy[i].pos.y - ENEMY_HEIGHT4 / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[i].pos.x - ENEMY_WIDTH4 / 2, g_aEnemy[i].pos.y + ENEMY_HEIGHT4 / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[i].pos.x + ENEMY_WIDTH4 / 2, g_aEnemy[i].pos.y + ENEMY_HEIGHT4 / 2, 0.0f);

				break;
			}

			g_pVtxBuffEnemy->Unlock();//バッファのアンロック
			g_nNumEnemy++;
			break;
		}
	}
}

//-------------------
//ヒット処理
//-------------------
void HitEnemy(int nCntEnemy, int nDamage)
{
	VERTEX_2D* pVtx;

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_aEnemy->fLength);
		g_aEnemy[nCntEnemy].bUse = false;
		g_nNumEnemy--;
		AddScore(nDamage*SCORE_DES);
	}
	else
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = 5;
		AddScore(nDamage*SCORE_HIT);

		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += nCntEnemy * VT_MAX;
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		g_pVtxBuffEnemy->Unlock();//バッファのアンロック
	}
}

//---------------------
//敵情報取得
//---------------------
Enemy* GetEnemy(void)
{
	return &g_aEnemy[0];
}

//----------------------
//敵の数の取得
//----------------------
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}