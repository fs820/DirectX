//---------------------------------------
//
//�e�\������[bullet.cpp]
//Author fuma sato
//
//---------------------------------------

#include"bullet.h"
#include"effect.h"
#include"exef.h"
#include"explosion.h"
#include"player.h"
#include"player2.h"
#include"democpu.h"
#include"enemy.h"
#include"boss.h"

#define BULLET_DAMAGE (10)
#define BULLET3_DAMAGE (30)
#define BULLETALL_DAMAGE (2)
#define BULLETSEARCH_DAMAGE (1)
#define SEARCH_TIME (30)
#define NOSEARCH_TIME (10)

//�e�\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	float fLength;
	D3DXVECTOR3 move;//�ړ���
	int nLife;//����
	BULLETTYPE Type;
	int nType;
	bool bUse;//�g�p���Ă��邩�ǂ���
	bool bSearch;
}Bullet;

//�O���[�o��
LPDIRECT3DTEXTURE9 g_apTextureBullet = NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;//�o�b�t�@�̃|�C���^
Bullet g_aBullet[MAX_BULLET];//�e�̏��

//-----------------
//�e�̏���������
//-----------------
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//�f�o�C�X���擾
	pDevice = GetDevice();

	for (int i = 0; i < MAX_BULLET; i++)
	{
		g_aBullet[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[i].fLength = 0.0f;
		g_aBullet[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[i].nLife = BULLET_LIFE;
		g_aBullet[i].Type = BULLETTYPE_PLAYER;
		g_aBullet[i].nType = 0;
		g_aBullet[i].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
		g_aBullet[i].bSearch = false;
	}

	//�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BULLET,
		&g_apTextureBullet
	);

	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_BULLET; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`��
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += VT_MAX;//�|���S��1���i�߂�
	}
	g_pVtxBuffBullet->Unlock();//�o�b�t�@�̃A�����b�N
}

//----------------
//�e�̏I������
//----------------
void UninitBullet(void)
{
	for (int i = 0; i < MAX_BULLET_TYPE; i++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureBullet != NULL)
		{
			g_apTextureBullet->Release();
			g_apTextureBullet = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//----------------
//�e�̍X�V����
//----------------
void UpdateBullet(void)
{
	VERTEX_2D* pVtx;

	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_aBullet[i].bUse == true)
		{//�e���g�p����Ă���
			if (g_aBullet[i].Type == BULLETTYPE_PLAYER)
			{//�v���C���[�̒e
				Enemy* pEnemy;
				Boss* pBoss;
				Player* pPlayer2,*pPlayer;

				pEnemy = GetEnemy();
				pBoss = GetBoss();
				pPlayer2 = GetPlayer2();
				pPlayer = GetPlayer();
				float EnemyWidth = 0.0f, EnemyHeight = 0.0f;
				for (int i2 = 0; i2 < MAX_ENEMY; i2++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���
						switch (pEnemy->nType)
						{
						case 0:
							EnemyWidth = ENEMY_WIDTH;
							EnemyHeight = ENEMY_HEIGHT;
							break;
						case 1:
							EnemyWidth = ENEMY_WIDTH2;
							EnemyHeight = ENEMY_HEIGHT2;
							break;
						case 2:
							EnemyWidth = ENEMY_WIDTH3;
							EnemyHeight = ENEMY_HEIGHT3;
							break;
						case 3:
							EnemyWidth = ENEMY_WIDTH4;
							EnemyHeight = ENEMY_HEIGHT4;
							break;
						}

						if (pEnemy->pos.x - EnemyWidth / 2 <= g_aBullet[i].pos.x + g_aBullet[i].fLength / 10.0f && pEnemy->pos.x + EnemyWidth / 2 >= g_aBullet[i].pos.x - g_aBullet[i].fLength / 10.0f && pEnemy->pos.y - EnemyHeight / 2 <= g_aBullet[i].pos.y + g_aBullet[i].fLength / 10.0f && pEnemy->pos.y + EnemyHeight / 2 >= g_aBullet[i].pos.y - g_aBullet[i].fLength / 10.0f)
						{
							int nDamage;

							switch (g_aBullet[i].nType)
							{
							case 0:
								nDamage = (int)((float)BULLET_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 1:
								nDamage = (int)((float)BULLET3_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 2:
								nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 3:
								nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							}

							HitEnemy(i2, nDamage);
							g_aBullet[i].bUse = false;
						}
						else if (g_aBullet[i].nType == 3)
						{
							float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f };
							static int SCount[MAX_BULLET] = { 0 };
							static bool bSa[MAX_BULLET] = { false };
							if (!bSa[i])
							{
								SCount[i]++;
								if (SCount[i] % NOSEARCH_TIME == 0)
								{
									SCount[i] = 0;
									bSa[i] = true;
								}
							}
							else
							{
								Xlong[i] = pEnemy->pos.x - g_aBullet[i].pos.x;
								Ylong[i] = pEnemy->pos.y - g_aBullet[i].pos.y;

								g_aBullet[i].move.x = sinf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;
								g_aBullet[i].move.y = cosf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;

								for (int i2 = 0; i2 < MAX_BULLET; i2++)
								{
									if (g_aBullet[i2].bUse == false)
									{
										bSa[i2] = false;
									}
								}
							}
						}
					}
				}

				for (int i2 = 0; i2 < MAX_BOSS; i2++, pBoss++)
				{
					if (pBoss->bUse == true)
					{//�G���g�p����Ă���
						switch (pBoss->nType)
						{
						case 0:
							EnemyWidth = BOSS_WIDTH;
							EnemyHeight = BOSS_HEIGHT;
							break;
						case 1:
							EnemyWidth = BOSS_WIDTH2;
							EnemyHeight = BOSS_HEIGHT2;
							break;
						case 2:
							EnemyWidth = BOSS_WIDTH3;
							EnemyHeight = BOSS_HEIGHT3;
							break;
						case 3:
							EnemyWidth = BOSS_WIDTH4;
							EnemyHeight = BOSS_HEIGHT4;
							break;
						}

						if (pBoss->pos.x - EnemyWidth / 2 <= g_aBullet[i].pos.x + g_aBullet[i].fLength / 10.0f && pBoss->pos.x + EnemyWidth / 2 >= g_aBullet[i].pos.x - g_aBullet[i].fLength / 10.0f && pBoss->pos.y - EnemyHeight / 2 <= g_aBullet[i].pos.y + g_aBullet[i].fLength / 10.0f && pBoss->pos.y + EnemyHeight / 2 >= g_aBullet[i].pos.y - g_aBullet[i].fLength / 10.0f)
						{
							int nDamage;

							switch (g_aBullet[i].nType)
							{
							case 0:
								nDamage = (int)((float)BULLET_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 1:
								nDamage = (int)((float)BULLET3_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 2:
								nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							case 3:
								nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer->fLength * 0.01f);
								break;
							}

   							HitBoss(i2, nDamage);
							g_aBullet[i].bUse = false;
						}
						else if (g_aBullet[i].nType == 3)
						{
							float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f };
							static int SCount[MAX_BULLET] = { 0 };
							static bool bSa[MAX_BULLET] = { false };
							if (!bSa[i])
							{
								SCount[i]++;
								if (SCount[i] % NOSEARCH_TIME == 0)
								{
									SCount[i] = 0;
									bSa[i] = true;
								}
							}
							else
							{
								Xlong[i] = pBoss->pos.x - g_aBullet[i].pos.x;
								Ylong[i] = pBoss->pos.y - g_aBullet[i].pos.y;

								g_aBullet[i].move.x = sinf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;
								g_aBullet[i].move.y = cosf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;

								for (int i2 = 0; i2 < MAX_BULLET; i2++)
								{
									if (g_aBullet[i2].bUse == false)
									{
										bSa[i2] = false;
									}
								}
							}
						}
					}
				}

				if (GetMode()==MODE_VS)
				{
					if (g_aBullet[i].nType == 3)
					{
						float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f };
						static int SCount[MAX_BULLET] = { 0 };
						static bool bSa[MAX_BULLET] = { false };
						if (!bSa[i])
						{
							SCount[i]++;
							if (SCount[i] % NOSEARCH_TIME == 0)
							{
								SCount[i] = 0;
								bSa[i] = true;
							}
						}
						else
						{
							Xlong[i] = pPlayer2->pos.x - g_aBullet[i].pos.x;
							Ylong[i] = pPlayer2->pos.y - g_aBullet[i].pos.y;

							g_aBullet[i].move.x = sinf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;
							g_aBullet[i].move.y = cosf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;

							for (int i2 = 0; i2 < MAX_BULLET; i2++)
							{
								if (g_aBullet[i2].bUse == false)
								{
									bSa[i2] = false;
								}
							}
						}
					}
				}

				if (pPlayer2->pos.x - pPlayer2->fLength <= g_aBullet[i].pos.x + g_aBullet[i].fLength / 10.0f && pPlayer2->pos.x + pPlayer2->fLength >= g_aBullet[i].pos.x - g_aBullet[i].fLength / 10.0f && pPlayer2->pos.y - pPlayer2->fLength <= g_aBullet[i].pos.y + g_aBullet[i].fLength / 10.0f && pPlayer2->pos.y + pPlayer2->fLength >= g_aBullet[i].pos.y - g_aBullet[i].fLength / 10.0f)
				{
					int nDamage;

					switch (g_aBullet[i].nType)
					{
					case 0:
						nDamage = (int)((float)BULLET_DAMAGE * pPlayer->fLength * 0.01f);
						break;
					case 1:
						nDamage = (int)((float)BULLET3_DAMAGE * pPlayer->fLength * 0.01f);
						break;
					case 2:
						nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer->fLength * 0.01f);
						break;
					case 3:
						nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer->fLength * 0.01f);
						break;
					}

					HitPlayer2(nDamage);
					g_aBullet[i].bUse = false;
				}

			}
			else if (g_aBullet[i].Type == BULLETTYPE_PLAYER2)
			{//�v���C���[�̒e
				Enemy* pEnemy;
				Boss* pBoss;
				Player* pPlayer2, * pPlayer;

				pEnemy = GetEnemy();
				pBoss = GetBoss();
				pPlayer2 = GetPlayer2();
				pPlayer = GetPlayer();
				float EnemyWidth = 0.0f, EnemyHeight = 0.0f;
				for (int i2 = 0; i2 < MAX_ENEMY; i2++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���
						switch (pEnemy->nType)
						{
						case 0:
							EnemyWidth = ENEMY_WIDTH;
							EnemyHeight = ENEMY_HEIGHT;
							break;
						case 1:
							EnemyWidth = ENEMY_WIDTH2;
							EnemyHeight = ENEMY_HEIGHT2;
							break;
						case 2:
							EnemyWidth = ENEMY_WIDTH3;
							EnemyHeight = ENEMY_HEIGHT3;
							break;
						case 3:
							EnemyWidth = ENEMY_WIDTH4;
							EnemyHeight = ENEMY_HEIGHT4;
							break;
						}

						if (pEnemy->pos.x - EnemyWidth / 2 <= g_aBullet[i].pos.x + g_aBullet[i].fLength / 10.0f && pEnemy->pos.x + EnemyWidth / 2 >= g_aBullet[i].pos.x - g_aBullet[i].fLength / 10.0f && pEnemy->pos.y - EnemyHeight / 2 <= g_aBullet[i].pos.y + g_aBullet[i].fLength / 10.0f && pEnemy->pos.y + EnemyHeight / 2 >= g_aBullet[i].pos.y - g_aBullet[i].fLength / 10.0f)
						{
							int nDamage;

							switch (g_aBullet[i].nType)
							{
							case 0:
								nDamage = (int)((float)BULLET_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 1:
								nDamage = (int)((float)BULLET3_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 2:
								nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 3:
								nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							}

							HitEnemy(i2, nDamage);
							g_aBullet[i].bUse = false;
						}
						else if (g_aBullet[i].nType == 3)
						{
							float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f };
							static int SCount[MAX_BULLET] = { 0 };
							static bool bSa[MAX_BULLET] = { false };
							if (!bSa[i])
							{
								SCount[i]++;
								if (SCount[i] % NOSEARCH_TIME == 0)
								{
									SCount[i] = 0;
									bSa[i] = true;
								}
							}
							else
							{
								Xlong[i] = pEnemy->pos.x - g_aBullet[i].pos.x;
								Ylong[i] = pEnemy->pos.y - g_aBullet[i].pos.y;

								g_aBullet[i].move.x = sinf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;
								g_aBullet[i].move.y = cosf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;

								for (int i2 = 0; i2 < MAX_BULLET; i2++)
								{
									if (g_aBullet[i2].bUse == false)
									{
										bSa[i2] = false;
									}
								}
							}
						}
					}
				}

				for (int i2 = 0; i2 < MAX_BOSS; i2++, pBoss++)
				{
					if (pBoss->bUse == true)
					{//�G���g�p����Ă���
						switch (pBoss->nType)
						{
						case 0:
							EnemyWidth = BOSS_WIDTH;
							EnemyHeight = BOSS_HEIGHT;
							break;
						case 1:
							EnemyWidth = BOSS_WIDTH2;
							EnemyHeight = BOSS_HEIGHT2;
							break;
						case 2:
							EnemyWidth = BOSS_WIDTH3;
							EnemyHeight = BOSS_HEIGHT3;
							break;
						case 3:
							EnemyWidth = BOSS_WIDTH4;
							EnemyHeight = BOSS_HEIGHT4;
							break;
						}

						if (pBoss->pos.x - EnemyWidth / 2 <= g_aBullet[i].pos.x + g_aBullet[i].fLength / 10.0f && pBoss->pos.x + EnemyWidth / 2 >= g_aBullet[i].pos.x - g_aBullet[i].fLength / 10.0f && pBoss->pos.y - EnemyHeight / 2 <= g_aBullet[i].pos.y + g_aBullet[i].fLength / 10.0f && pBoss->pos.y + EnemyHeight / 2 >= g_aBullet[i].pos.y - g_aBullet[i].fLength / 10.0f)
						{
							int nDamage;

							switch (g_aBullet[i].nType)
							{
							case 0:
								nDamage = (int)((float)BULLET_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 1:
								nDamage = (int)((float)BULLET3_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 2:
								nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							case 3:
								nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer2->fLength * 0.01f);
								break;
							}

							HitBoss(i2, nDamage);
							g_aBullet[i].bUse = false;
						}
						else if (g_aBullet[i].nType == 3)
						{
							float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f };
							static int SCount[MAX_BULLET] = { 0 };
							static bool bSa[MAX_BULLET] = { false };
							if (!bSa[i])
							{
								SCount[i]++;
								if (SCount[i] % NOSEARCH_TIME == 0)
								{
									SCount[i] = 0;
									bSa[i] = true;
								}
							}
							else
							{
								Xlong[i] = pBoss->pos.x - g_aBullet[i].pos.x;
								Ylong[i] = pBoss->pos.y - g_aBullet[i].pos.y;

								g_aBullet[i].move.x = sinf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;
								g_aBullet[i].move.y = cosf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;

								for (int i2 = 0; i2 < MAX_BULLET; i2++)
								{
									if (g_aBullet[i2].bUse == false)
									{
										bSa[i2] = false;
									}
								}
							}
						}
					}
				}

				if (GetMode() == MODE_VS)
				{
					if (g_aBullet[i].nType == 3)
					{
						float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f };
						static int SCount[MAX_BULLET] = { 0 };
						static bool bSa[MAX_BULLET] = { false };
						if (!bSa[i])
						{
							SCount[i]++;
							if (SCount[i] % NOSEARCH_TIME == 0)
							{
								SCount[i] = 0;
								bSa[i] = true;
							}
						}
						else
						{
							Xlong[i] = pPlayer->pos.x - g_aBullet[i].pos.x;
							Ylong[i] = pPlayer->pos.y - g_aBullet[i].pos.y;

							g_aBullet[i].move.x = sinf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;
							g_aBullet[i].move.y = cosf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;

							for (int i2 = 0; i2 < MAX_BULLET; i2++)
							{
								if (g_aBullet[i2].bUse == false)
								{
									bSa[i2] = false;
								}
							}
						}
					}
				}

				if (pPlayer->pos.x - pPlayer->fLength <= g_aBullet[i].pos.x + g_aBullet[i].fLength / 10.0f && pPlayer->pos.x + pPlayer->fLength >= g_aBullet[i].pos.x - g_aBullet[i].fLength / 10.0f && pPlayer->pos.y - pPlayer->fLength <= g_aBullet[i].pos.y + g_aBullet[i].fLength / 10.0f && pPlayer->pos.y + pPlayer->fLength >= g_aBullet[i].pos.y - g_aBullet[i].fLength / 10.0f)
				{
					int nDamage;

					switch (g_aBullet[i].nType)
					{
					case 0:
						nDamage = (int)((float)BULLET_DAMAGE * pPlayer2->fLength * 0.01f);
						break;
					case 1:
						nDamage = (int)((float)BULLET3_DAMAGE * pPlayer2->fLength * 0.01f);
						break;
					case 2:
						nDamage = (int)((float)BULLETALL_DAMAGE * pPlayer2->fLength * 0.01f);
						break;
					case 3:
						nDamage = (int)((float)BULLETSEARCH_DAMAGE * pPlayer2->fLength * 0.01f);
						break;
					}

					HitPlayer(nDamage);
					g_aBullet[i].bUse = false;
				}

			}
			else if (g_aBullet[i].Type == BULLETTYPE_ENEMY)
			{//�G�̒e
				Player* pPlayer;

				pPlayer = GetPlayer();
				if (pPlayer->pos.x - pPlayer->fLength <= g_aBullet[i].pos.x + g_aBullet[i].fLength / 10.0f && pPlayer->pos.x + pPlayer->fLength >= g_aBullet[i].pos.x - g_aBullet[i].fLength / 10.0f && pPlayer->pos.y - pPlayer->fLength <= g_aBullet[i].pos.y + g_aBullet[i].fLength / 10.0f && pPlayer->pos.y + pPlayer->fLength >= g_aBullet[i].pos.y - g_aBullet[i].fLength / 10.0f)
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
					case 3:
						nDamage = BULLETSEARCH_DAMAGE;
						break;
					}

					if (GetMode()==MODE_DEMO)
					{
						HitDemoCpu(nDamage);
					}
					else
					{
						HitPlayer(nDamage);
					}
					g_aBullet[i].bUse = false;
				}
				else if (g_aBullet[i].nType == 3)
				{
					static bool bSa[MAX_BULLET] = { true };
					static int nSaCnt[MAX_BULLET] = { 0 };
					float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f };
					nSaCnt[i]++;

					Xlong[i] = pPlayer->pos.x - g_aBullet[i].pos.x;
					Ylong[i] = pPlayer->pos.y - g_aBullet[i].pos.y;

					if (nSaCnt[i] % SEARCH_TIME == 0)
					{
						nSaCnt[i] = 0;
						bSa[i] = false;
					}

					if (bSa[i])
					{
						g_aBullet[i].move.x = sinf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;
						g_aBullet[i].move.y = cosf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;
					}

					for (int i2 = 0; i2 < MAX_BULLET; i2++)
					{
						if (g_aBullet[i2].bUse == false)
						{
							bSa[i2] = true;
						}
					}
				}
			}
			else if (g_aBullet[i].Type == BULLETTYPE_BOSS)
			{//�G�̒e
				Player* pPlayer;

				pPlayer = GetPlayer();
				if (pPlayer->pos.x - pPlayer->fLength <= g_aBullet[i].pos.x + g_aBullet[i].fLength / 10.0f && pPlayer->pos.x + pPlayer->fLength >= g_aBullet[i].pos.x - g_aBullet[i].fLength / 10.0f && pPlayer->pos.y - pPlayer->fLength <= g_aBullet[i].pos.y + g_aBullet[i].fLength / 10.0f && pPlayer->pos.y + pPlayer->fLength >= g_aBullet[i].pos.y - g_aBullet[i].fLength / 10.0f)
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
					case 3:
						nDamage = BULLETSEARCH_DAMAGE;
						break;
					}

					if (GetMode() == MODE_DEMO)
					{
						HitDemoCpu(nDamage);
					}
					else
					{
						HitPlayer(nDamage);
					}
					g_aBullet[i].bUse = false;
				}
				else if (g_aBullet[i].nType == 3)
				{
					static bool bSa[MAX_BULLET] = { true };
					static int nSaCnt[MAX_BULLET] = { 0 };
					float Xlong[MAX_BULLET] = { 0.0f }, Ylong[MAX_BULLET] = { 0.0f };
					nSaCnt[i]++;

					Xlong[i] = pPlayer->pos.x - g_aBullet[i].pos.x;
					Ylong[i] = pPlayer->pos.y - g_aBullet[i].pos.y;

					if (nSaCnt[i] % SEARCH_TIME == 0)
					{
						nSaCnt[i] = 0;
						bSa[i] = false;
					}

					if (bSa[i])
					{
						g_aBullet[i].move.x = sinf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;
						g_aBullet[i].move.y = cosf(atan2f(Xlong[i], Ylong[i])) * BULLET_SPEED;
					}

					for (int i2 = 0; i2 < MAX_BULLET; i2++)
					{
						if (g_aBullet[i2].bUse == false)
						{
							bSa[i2] = true;
						}
					}
				}
			}

			g_aBullet[i].nLife--;

			g_aBullet[i].pos.x += g_aBullet[i].move.x;
			g_aBullet[i].pos.y += g_aBullet[i].move.y;

			g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

			//���W�ݒ�
			pVtx += i * VT_MAX;//�|���S��1���i�߂�
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);

			g_pVtxBuffBullet->Unlock();//�o�b�t�@�̃A�����b�N

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
				SetExef(g_aBullet[i].pos,g_aBullet[i].fLength);
				g_aBullet[i].bUse = false;
			}

			D3DXCOLOR col;
			switch (g_aBullet[i].nType)
			{
			case 0:
				col = D3DXCOLOR(0.8f, 0.0f, 0.0f, (float)(rand() % 51) / 100.0f);
				break;
			case 1:
				col = D3DXCOLOR(0.0f, 0.8f, 0.0f, (float)(rand() % 51) / 100.0f);
				break;
			case 2:
				col = D3DXCOLOR(0.0f, 0.0f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			case 3:
				col = D3DXCOLOR(0.0f, 0.8f, 0.8f, (float)(rand() % 51) / 100.0f);
				break;
			}

			SetEffect(g_aBullet[i].pos,D3DXVECTOR3(0.0f,0.0f,0.0f), col, g_aBullet[i].fLength / 5.0f, 100);
		}
	}
}

//----------------
//�e�̕`�揈��
//----------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_aBullet[i].bUse == true)
		{//�e���g�p����Ă���

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureBullet);
			//�e�̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//�^�C�v
				i * VT_MAX,//�n�܂�̔ԍ�
				2//�|���S���̌�
			);
		}
	}
}

//-------------------
//���ʏ�e
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
			{//�e���g�p����Ă��Ȃ�
				g_aBullet[i].pos = pos;
				g_aBullet[i].fLength = fLengthPlayer;

				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

				//���_���W�ݒ�
				pVtx += i * VT_MAX;//�|���S��1���i�߂�
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);

				g_pVtxBuffBullet->Unlock();//�o�b�t�@�̃A�����b�N

				g_aBullet[i].move = move;
				g_aBullet[i].nLife = nLife;
				g_aBullet[i].Type = type;
				g_aBullet[i].nType = 0;
				g_aBullet[i].bUse = true;
				g_aBullet[i].bSearch = false;

				break;
			}
		}
	}
}

//-----------------
//3wey�e
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
			{//�e���g�p����Ă��Ȃ�
				g_aBullet[i].pos = pos;
				g_aBullet[i + 1].pos = pos;
				g_aBullet[i + 2].pos = pos;
				g_aBullet[i].fLength = fLengthPlayer;
				g_aBullet[i + 1].fLength = fLengthPlayer;
				g_aBullet[i + 2].fLength = fLengthPlayer;

				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

				//���_���W�ݒ�
				pVtx += i * VT_MAX;//�|���S��1���i�߂�
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx += VT_MAX;//�|���S��1���i�߂�
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx += VT_MAX;//�|���S��1���i�߂�
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[i].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[i].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i].fLength / 100.0f), g_aBullet[i].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i].fLength / 100.0f), 0.0f);

				g_pVtxBuffBullet->Unlock();//�o�b�t�@�̃A�����b�N

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
//�S���ʒe
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
				pVtx += i * VT_MAX;//�|���S��1���i�߂�
				for (int i2 = 0; i2 < ALL_BULLET; i2++)
				{
					if (g_aBullet[i + i2].bUse == false)
					{//�e���g�p����Ă��Ȃ�

						g_aBullet[i + i2].pos = pos;
						g_aBullet[i + i2].fLength = fLengthPlayer;


						//���_���W�ݒ�
						pVtx[0].pos = D3DXVECTOR3(g_aBullet[i + i2].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aBullet[i + i2].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[2].pos = D3DXVECTOR3(g_aBullet[i + i2].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aBullet[i + i2].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);

						pVtx += VT_MAX;//�|���S��1���i�߂�

						g_aBullet[i + i2].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * fAll) * BULLET_SPEED, cosf(Playerrot + D3DX_PI * fAll) * BULLET_SPEED, 0.0f);

						g_aBullet[i + i2].nLife = nLife;
						g_aBullet[i + i2].Type = type;
						g_aBullet[i + i2].nType = 2;
						g_aBullet[i + i2].bUse = true;
						fAll += 2.0f / (float)ALL_BULLET;
					}
				}
				g_pVtxBuffBullet->Unlock();//�o�b�t�@�̃A�����b�N
				break;
			}
		}
	}
}

//-----------------
//�U���e
//-----------------
void SetSearchBullet(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, BULLETTYPE type, int nInter)
{
	VERTEX_2D* pVtx;
	float fAll = 0.0f;
	static int Inter = 0;
	bool OK = false;

	Inter++;

	if (Inter % nInter == 0)
	{
		nInter = 0;

		for (int i = 0; i < MAX_BULLET - ALL_BULLETS; i++)
		{
			for (int i2 = 0; i2 < ALL_BULLETS; i2++)
			{
				if (g_aBullet[i + i2].bUse == true)
				{
					break;
				}
				if (i2 == ALL_BULLETS - 1)
				{
					OK = true;
				}
			}
			if (OK == true)
			{
				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += i * VT_MAX;//�|���S��1���i�߂�
				for (int i2 = 0; i2 < ALL_BULLETS; i2++)
				{
					if (g_aBullet[i + i2].bUse == false)
					{//�e���g�p����Ă��Ȃ�

						g_aBullet[i + i2].pos = pos;
						g_aBullet[i + i2].fLength = fLengthPlayer;


						//���_���W�ݒ�
						pVtx[0].pos = D3DXVECTOR3(g_aBullet[i + i2].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aBullet[i + i2].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos.y - BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[2].pos = D3DXVECTOR3(g_aBullet[i + i2].pos.x - BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aBullet[i + i2].pos.x + BULLET_WIDTH / 2 * (g_aBullet[i + i2].fLength / 100.0f), g_aBullet[i + i2].pos.y + BULLET_HEIGHT / 2 * (g_aBullet[i + i2].fLength / 100.0f), 0.0f);

						pVtx += VT_MAX;//�|���S��1���i�߂�

						g_aBullet[i + i2].move = D3DXVECTOR3(sinf(Playerrot + D3DX_PI * fAll) * BULLET_SPEED, cosf(Playerrot + D3DX_PI * fAll) * BULLET_SPEED, 0.0f);

						g_aBullet[i + i2].nLife = nLife;
						g_aBullet[i + i2].Type = type;
						g_aBullet[i + i2].nType = 3;
						g_aBullet[i + i2].bUse = true;
						fAll += 2.0f / (float)ALL_BULLETS;
					}
				}
				g_pVtxBuffBullet->Unlock();//�o�b�t�@�̃A�����b�N
				break;
			}
		}
	}
}