//---------------------------------------
//
//デモプレイコンピューター表示処理の定義・宣言[democpu.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _DEMOCPU_H_
#define _DEMOCPU_H_

#include"main.h"
#include"player.h"

//プレイヤー情報
#define DEMOCPU_WIDTH (160)//幅
#define DEMOCPU_HEIGHT (90)//高さ
#define DEMOCPU_SPEED_DEF (0.0f)
#define DEMOCPU_SPEED_X (3.0f)//速さ
#define DEMOCPU_SPEED_Y (3.0f)
#define DEMOCPU_INA (0.2f)
#define DEMOCPU_ROT_SPEED (0.05f)
#define DEMOCPU_SIZE_SPEED (3.0f)
#define DEMOCPU_SIZE_MAX (300.0f)
#define DEMOCPU_SIZE_MIN (5.0f)

#define U_MAX_C (1)//テクスチャの数(横)
#define V_MAX_C (1)//テクスチャの数(縦)

typedef struct
{
	int nCounterAnim;//プレイヤーの更新間隔
	int nPatternAnim;//プレイヤーのアニメーションパターン管理
	D3DXVECTOR3 pos;//プレイヤー座標管理
	D3DXVECTOR3 move;//プレイヤースピード管理
	D3DXVECTOR3 rot;//プレイヤー向き管理
	D3DXVECTOR3 rotDef;//プレイヤー向き管理
	float fLength;//対角線の長さ(大きさ)
	float fLengthDef;//対角線の長さ(大きさ)の初期値
	float fAngle;//対角線の角度
	int nLife;//体力
	PLAYERSTATE state;//状態
	int nCounterState;
}DEMOCPU;

void InitDemoCpu(void);//ポリゴンの初期化処理
void UninitDemoCpu(void);//ポリゴンの終了処理
void UpdateDemoCpu(void);//ポリゴンの更新処理
void DrawDemoCpu(void);//ポリゴンの描画処理
void HitDemoCpu(int nDamage);//ヒット処理
PDIRECT3DVERTEXBUFFER9 GetDemoCpuBuffer(void);//プレイヤーバッファの取得
DEMOCPU* GetDemoCpu(void);

#endif _DEMOCPU_H_