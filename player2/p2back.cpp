//---------------------------------------
//
//�w�i�\������[p2back.cpp]
//Author fuma sato
//
//---------------------------------------

#include"main.h"
#include"p2back.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureP2Back = NULL;//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffP2Back = NULL;//�o�b�t�@�̃|�C���^
float g_PosTexUP2;
float g_PosTexVP2;
int g_nCounterAnimP2Back;//�X�V�Ԋu�Ǘ�
int g_nPatternAnimP2Back;//�e�N�X�`���p�^�[���Ǘ�

//----------------------
//�w�i�̏���������
//----------------------
void InitP2Back(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^
	VERTEX_2D* pVtx;//���_���|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffP2Back,
		NULL
	);

	//1��

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_FIELD,
		&g_pTextureP2Back
	);

	g_PosTexUP2 = 0.0f;
	g_PosTexVP2 = 0.0f;

	g_nCounterAnimP2Back = 0;
	g_nPatternAnimP2Back = 0;

	g_pVtxBuffP2Back->Lock(0, 0, (void**)&pVtx, 0);//�o�b�t�@�̃��b�N

		//���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH-SCREEN_WIDTH/4, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - SCREEN_WIDTH / 4, SCREEN_HEIGHT/4, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT/4, 0.0f);

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
	pVtx[0].tex = D3DXVECTOR2(g_PosTexUP2, g_PosTexVP2);
	pVtx[1].tex = D3DXVECTOR2(g_PosTexUP2 + 1.0f, g_PosTexVP2);
	pVtx[2].tex = D3DXVECTOR2(g_PosTexUP2, g_PosTexVP2 + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_PosTexUP2 + 1.0f, g_PosTexVP2 + 1.0f);

	g_pVtxBuffP2Back->Unlock();//�o�b�t�@�̃A�����b�N
}

//-------------------
//�w�i�I������
//-------------------
void UninitP2Back(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureP2Back != NULL)
	{
		g_pTextureP2Back->Release();
		g_pTextureP2Back = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffP2Back != NULL)
	{
		g_pVtxBuffP2Back->Release();
		g_pVtxBuffP2Back = NULL;
	}
}

//-------------------
//�w�i�X�V����
//-------------------
void UpdateP2Back(void)
{
	VERTEX_2D* pVtx;//���_���|�C���^

	g_pVtxBuffP2Back->Lock(0, 0, (void**)&pVtx, 0);//�o�b�t�@�̃��b�N

	int nRand = 0;
	nRand = rand() % 4;
	switch (nRand)
	{
	case 0:
		g_PosTexUP2 += ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		g_PosTexVP2 += ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		break;
	case 1:
		g_PosTexUP2 += ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		g_PosTexVP2 -= ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		break;
	case 2:
		g_PosTexUP2 -= ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		g_PosTexVP2 += ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		break;
	case 3:
		g_PosTexUP2 -= ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		g_PosTexVP2 -= ((float)(rand() % 11) / 10.0f + 0.1f) * 0.01f;
		break;
	}

	//�e�N�X�`��
	pVtx[0].tex = D3DXVECTOR2(g_PosTexUP2, g_PosTexVP2);
	pVtx[1].tex = D3DXVECTOR2(g_PosTexUP2 + 1.0f, g_PosTexVP2);
	pVtx[2].tex = D3DXVECTOR2(g_PosTexUP2, g_PosTexVP2 + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_PosTexUP2 + 1.0f, g_PosTexVP2 + 1.0f);

	g_pVtxBuffP2Back->Unlock();//�o�b�t�@�̃A�����b�N
}

//-------------------
//�w�i�`�揈��
//-------------------
void DrawP2Back(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�փ|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->SetStreamSource(0, g_pVtxBuffP2Back, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureP2Back);

	//�w�i�̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//�^�C�v
		0,//�n�܂�̔ԍ�
		2//�|���S���̌�
	);
}