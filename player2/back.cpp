//---------------------------------------
//
//�w�i�\������[back.cpp]
//Author fuma sato
//
//---------------------------------------

#include"main.h"
#include"back.h"

#define NUM_BG (3)
#define BACK_SPEED (0.05f)

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureBack[NUM_BG] = { NULL };//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBack = NULL;//�o�b�t�@�̃|�C���^
float g_aPosTexU[NUM_BG];
int g_nCounterAnimBack;//�X�V�Ԋu�Ǘ�
int g_nPatternAnimBack;//�e�N�X�`���p�^�[���Ǘ�

//----------------------
//�w�i�̏���������
//----------------------
void InitBack(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBack,
		NULL
	);

	//1��

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BG1,
		&g_apTextureBack[0]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_BG2,
		&g_apTextureBack[1]
	);

	//�e�N�X�`���̓ǂݍ���
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

	g_pVtxBuffBack->Lock(0, 0, (void**)&pVtx, 0);//�o�b�t�@�̃��b�N

	for (int i = 0; i < NUM_BG; i++)
	{
		//���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(g_aPosTexU[i], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aPosTexU[i] + 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aPosTexU[i], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aPosTexU[i] + 1.0f, 1.0f);

		pVtx += VT_MAX;
	}

	g_pVtxBuffBack->Unlock();//�o�b�t�@�̃A�����b�N
}

//-------------------
//�w�i�I������
//-------------------
void UninitBack(void)
{
	for (int i = 0; i < NUM_BG; i++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureBack[i] != NULL)
		{
			g_apTextureBack[i]->Release();
			g_apTextureBack[i] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBack != NULL)
	{
		g_pVtxBuffBack->Release();
		g_pVtxBuffBack = NULL;
	}
}

//-------------------
//�w�i�X�V����
//-------------------
void UpdateBack(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^

	g_pVtxBuffBack->Lock(0, 0, (void**)&pVtx, 0);//�o�b�t�@�̃��b�N

	for (int i = 0; i < NUM_BG; i++)
	{
		g_aPosTexU[i] += BACK_SPEED * ((i + 0.1f) * 0.1f);

		//�e�N�X�`��
		pVtx[0].tex = D3DXVECTOR2(g_aPosTexU[i], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aPosTexU[i] + 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aPosTexU[i], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aPosTexU[i] + 1.0f, 1.0f);

		pVtx += VT_MAX;
	}

	g_pVtxBuffBack->Unlock();//�o�b�t�@�̃A�����b�N
}

//-------------------
//�w�i�`�揈��
//-------------------
void DrawBack(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffBack, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < NUM_BG; i++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureBack[i]);

		//�w�i�̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//�^�C�v
			VT_MAX * i,//�n�܂�̔ԍ�
			2//�|���S���̌�
		);
	}
}