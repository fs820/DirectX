//---------------------------------------
//
//弾表示処理[bullet.cpp]
//Author fuma sato
//
//---------------------------------------

#include"bullet.h"
#include"effect.h"
#include"explosion.h"
#include"player.h"
#include"enemy.h"

#define BULLET_DAMAGE (10)
#define BULLET3_DAMAGE (30)
#define BULLETALL_DAMAGE (2)

//弾構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	float fLength;
	D3DXVECTOR3 move;//移動量
	int nLife;//寿命
	BULLETTYPE Type;
	int nType;
	bool bUse;//使用しているかどうか
}Bullet;

//グローバル
LPDIRECT3DTEXTURE9 g_apTextureBullet[MAX_BULLET_TYPE] = { NULL };//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;//バッファのポインタ
Bullet g_aBullet[MAX_BULLET];//弾の情報

//-----------------
//弾の初期化処理
//-----------------
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//デバイスを取得
	pDevice = GetDevice();

	for (int i = 0; i < MAX_BULLET; i++)
	{
		g_aBullet[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[i].fLength = 0.0f;
		g_aBullet[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[i].nLife = BULLET_LIFE;
		g_aBullet[i].Type = BULLETTYPE_PLAYER;
		g_aBullet[i].nType = 0;
		g_aBullet[i].bUse = false;//使用していない状態にする
	}

	//バッファの作成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BULLET,
		&g_apTextureBullet[0]
	);
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BULLET2,
		&g_apTextureBullet[1]
	);
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BULLET3,
		&g_apTextureBullet[2]
	);

	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_BULLET; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);

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

		pVtx += VT_MAX;//ポリゴン1つ分進める
	}
	g_pVtxBuffBullet->Unlock();//バッファのアンロック
}

//----------------
//弾の終了処理
//----------------
void UninitBullet(void)
{
	for (int i = 0; i < MAX_BULLET_TYPE; i++)
	{
		//テクスチャの破棄
		if (g_apTextureBullet[i] != NULL)
		{
			g_apTextureBullet[i]->Release();
			g_apTextureBullet[i] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//----------------
//弾の更新処理
//----------------
void UpdateBullet(void)
{
	VERTEX_2D* pVtx;

	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_aBullet[i].bUse == true)
		{//弾が使用されている
			if (g_aBullet[i].Type == BULLETTYPE_PLAYER)
			{//プレイヤーの弾
				Enemy* pEnemy;

				pEnemy = GetEnemy();
				for (int i2 = 0; i2 < MAX_ENEMY; i2++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている
						if (pEnemy->pos.x - ENEMY_WIDTH / 2 <= g_aBullet[i].pos.x + g_aBullet[i].fLength / 10.0f && pEnemy->pos.x + ENEMY_WIDTH / 2 >= g_aBullet[i].pos.x - g_aBullet[i].fLength / 10.0f && pEnemy->pos.y - ENEMY_HEIGHT / 2 <= g_aBullet[i].pos.y + g_aBullet[i].fLength / 10.0f && pEnemy->pos.y + ENEMY_HEIGHT / 2 >= g_aBullet[i].pos.y - g_aBullet[i].fLength / 10.0f)
						{
							int nDamage;

							switch (g_aBullet[i].nType)
							{
							case 0:
								nDamage = BULLET_DAMAGE;
								break;
							case 1:
								nDamage = BULLET3_DAMAGE;
								break;
							case 2:
								nDamage = BULLETALL_DAMAGE;
								break;
							}

							HitEnemy(i2, nDamage);
							g_aBullet[i].bUse = false;
						}
					}
				}
			}
			else if (g_aBullet[i].Type == BULLETTYPE_ENEMY)
			{//敵の弾
				Player* pPlayer;

				pPlayer = GetPlayer();
				if (pPlayer->pos.x - PLAYER_WIDTH / 2 <= g_aBullet[i].pos.x + g_aBullet[i].fLength / 10.0f && pPlayer->pos.x + PLAYER_WIDTH / 2 >= g_aBullet[i].pos.x - g_aBullet[i].fLength / 10.0f && pPlayer->pos.y - PLAYER_HEIGHT / 2 <= g_aBullet[i].pos.y + g_aBullet[i].fLength / 10.0f && pPlayer->pos.y + PLAYER_HEIGHT / 2 >= g_aBullet[i].pos.y - g_aBullet[i].fLength / 10.0f)
				{
					int nDamage = 0;

					switch (g_aBullet[i].nType)
					{
					case 0:
						nDamage = BULLET_DAMAGE;
						break;
					case 1:
						nDamage = BULLET3_DAMAGE;
						break;
					case 2:
						nDamage = BULLETALL_DAMAGE;
						break;
					}

					HitPlayer(nDamage);
					g_aBullet[i].bUse = false;
				}
			}

			g_aBullet[i].nLife--;

			g_aBullet[i].pos.x += g_aBullet[i].move.x;
			g_aBullet[i].pos.y += g_aBullet[i].move.y;

			g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

			//座標設定
			pVtx += i * VT_MAX;//ポリゴン1つ分進める
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);

			g_pVtxBuffBullet->Unlock();//バッファのアンロック

			if (g_aBullet[i].pos.x + BULLET_WIDTH / 2 >= SCREEN_WIDTH)
			{
				g_aBullet[i].bUse = false;
			}
			else if (g_aBullet[i].pos.x - BULLET_WIDTH / 2 <= 0.0f)
			{
				g_aBullet[i].bUse = false;
			}
			else if (g_aBullet[i].pos.y + BULLET_HEIGHT / 2 >= SCREEN_HEIGHT)
			{
				g_aBullet[i].bUse = false;
			}
			else if (g_aBullet[i].pos.y - BULLET_HEIGHT / 2 <= 0.0f)
			{
				g_aBullet[i].bUse = false;
			}

			if (g_aBullet[i].nLife <= 0)
			{
				SetExplosion(g_aBullet[i].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aBullet[i].fLength);
				g_aBullet[i].bUse = false;
			}

			D3DXCOLOR col;
			switch (g_aBullet[i].nType)
			{
			case 0:
				col = D3DXCOLOR(0.8f, 0.0f, 0.0f, 0.2f);
				break;
			case 1:
				col = D3DXCOLOR(0.0f, 0.8f, 0.0f, 0.2f);
				break;
			case 2:
				col = D3DXCOLOR(0.0f, 0.0f, 0.8f, 0.2f);
				break;
			}

			D3DXVECTOR3 rot;
			rot.x=0.0f;
			rot.y=0.0f;
			rot.z = atan2f((float)rand(),(float)rand());

			SetEffect(g_aBullet[i].pos,rot, col, g_aBullet[i].fLength/5.0f, 100,i);
		}
	}
}

//----------------
//弾の描画処理
//----------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_aBullet[i].bUse == true)
		{//弾が使用されている

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureBullet[g_aBullet[i].nType]);

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
//発通常弾
//-------------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter)
{
	VERTEX_2D* pVtx;
	static int Inter = 0;

	Inter++;

	if (Inter % nInter == 0)
	{
		for (int i = 0; i < MAX_BULLET; i++)
		{
			if (g_aBullet[i].bUse == false)
			{//弾が使用されていない
				g_aBullet[i].pos = pos;
				g_aBullet[i].fLength = fLengthPlayer;

				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

				//頂点座標設定
				pVtx += i * VT_MAX;//ポリゴン1つ分進める
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);

				g_pVtxBuffBullet->Unlock();//バッファのアンロック

				g_aBullet[i].move = move;
				g_aBullet[i].nLife = nLife;
				g_aBullet[i].Type = type;
				g_aBullet[i].nType = 0;
				g_aBullet[i].bUse = true;

				break;
			}
		}
	}
}

//-----------------
//3wey弾
//-----------------
void Set3Bullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter)
{
	VERTEX_2D* pVtx;
	static int Inter = 0;

	Inter++;

	if (Inter % nInter == 0)
	{
		nInter = 0;


		for (int i = 0; i < MAX_BULLET - 2; i++)
		{
			if (g_aBullet[i].bUse == false && g_aBullet[i + 1].bUse == false && g_aBullet[i + 2].bUse == false)
			{//弾が使用されていない
				g_aBullet[i].pos = pos;
				g_aBullet[i + 1].pos = pos;
				g_aBullet[i + 2].pos = pos;
				g_aBullet[i].fLength = fLengthPlayer;
				g_aBullet[i + 1].fLength = fLengthPlayer;
				g_aBullet[i + 2].fLength = fLengthPlayer;

				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

				//頂点座標設定
				pVtx += i * VT_MAX;//ポリゴン1つ分進める
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx += VT_MAX;//ポリゴン1つ分進める
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx += VT_MAX;//ポリゴン1つ分進める
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);

				g_pVtxBuffBullet->Unlock();//バッファのアンロック

				g_aBullet[i].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.5f) * BULLET_SPEED, cosf(Playerrot + D3DX_PI * 0.5f) * BULLET_SPEED, 0.0f);
				g_aBullet[i + 1].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.25f) * BULLET_SPEED, cosf(Playerrot + D3DX_PI * 0.25f) * BULLET_SPEED, 0.0f);
				g_aBullet[i + 2].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * 0.75f) * BULLET_SPEED, cosf(Playerrot + D3DX_PI * 0.75f) * BULLET_SPEED, 0.0f);

				g_aBullet[i].nLife = nLife;
				g_aBullet[i + 1].nLife = nLife;
				g_aBullet[i + 2].nLife = nLife;
				g_aBullet[i].Type = type;
				g_aBullet[i + 1].Type = type;
				g_aBullet[i + 2].Type = type;
				g_aBullet[i].nType = 1;
				g_aBullet[i + 1].nType = 1;
				g_aBullet[i + 2].nType = 1;
				g_aBullet[i].bUse = true;
				g_aBullet[i + 1].bUse = true;
				g_aBullet[i + 2].bUse = true;

				break;
			}
		}
	}
}

//----------------
//全方位弾
//----------------
void SetAllBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter)
{
	VERTEX_2D* pVtx;
	float fAll = 0.0f;
	static int Inter = 0;
	bool OK = false;

	Inter++;

	if (Inter % nInter == 0)
	{
		nInter = 0;

		for (int i = 0; i < MAX_BULLET - ALL_BULLET; i++)
		{
			for (int i2 = 0; i2 < ALL_BULLET; i2++)
			{
				if (g_aBullet[i + i2].bUse == true)
				{
					break;
				}
				if (i2 == ALL_BULLET - 1)
				{
					OK = true;
				}
			}
			if (OK == true)
			{
				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += i * VT_MAX;//ポリゴン1つ分進める
				for (int i2 = 0; i2 < ALL_BULLET; i2++)
				{
					if (g_aBullet[i + i2].bUse == false)
					{//弾が使用されていない

						g_aBullet[i + i2].pos = pos;
						g_aBullet[i + i2].fLength = fLengthPlayer;


						//頂点座標設定
						pVtx[0].pos = D3DXVECTOR3(g_aBullet[i + i2].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aBullet[i + i2].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[2].pos = D3DXVECTOR3(g_aBullet[i + i2].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aBullet[i + i2].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);

						pVtx += VT_MAX;//ポリゴン1つ分進める

						g_aBullet[i + i2].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * fAll) * BULLET_SPEED, cosf(Playerrot + D3DX_PI * fAll) * BULLET_SPEED, 0.0f);

						g_aBullet[i + i2].nLife = nLife;
						g_aBullet[i + i2].Type = type;
						g_aBullet[i + i2].nType = 2;
						g_aBullet[i + i2].bUse = true;
						fAll += 2.0f / (float)ALL_BULLET;
					}
				}
				g_pVtxBuffBullet->Unlock();//バッファのアンロック
				break;
			}
		}
	}
}