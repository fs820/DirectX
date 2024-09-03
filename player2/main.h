//------------------------------------------
//
//テクスチャ表示プログラムのメインの処理の定義・宣言[main.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _MAIN_H_
#define _MAIN_H_

#include<windows.h>
#include<time.h>
#include"d3dx9.h"
#include"dinput.h" // 入力に必要なファイル
#include"Xinput.h" // Xinput(パッド処理)
#include"xaudio2.h"

// ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"uuid.lib")

// マクロ定義
#define DPS (1000/60) // 更新スピード
#define UP_TIME (3) // テクスチャスピード

// 名前
#define CLASS_NAME "WindowClass" // クラス名
#define WINDOW_NAME "「勇者は結局兵器が強いことに気付いたようです」になる予定のもの" // ウインドウ名
#define DATA_FILE "data\\DATA\\data.bin"
#define TEXTURE_CURSOR "data\\TEXTURE\\Cursor.png"
#define TEXTURE_TITLE "data\\TEXTURE\\title000.png"
#define TEXTURE_SELECT1 "data\\TEXTURE\\Enter or Start.png"
#define TEXTURE_SELECT2 "data\\TEXTURE\\Select.png"
#define TEXTURE_RESULT "data\\TEXTURE\\result000.png"
#define TEXTURE_SELECTRESTART "data\\TEXTURE\\Restart.png"
#define TEXTURE_NAME2 "data\\TEXTURE\\gita.png" // テクスチャ名2
#define TEXTURE_NAME_BULLET "data\\TEXTURE\\bullet000.png" // テクスチャ名2
#define TEXTURE_NAME_BG1 "data\\TEXTURE\\bg100.png"
#define TEXTURE_NAME_BG2 "data\\TEXTURE\\bg101.png"
#define TEXTURE_NAME_BG3 "data\\TEXTURE\\bg102.png"
#define TEXTURE_NAME_EXP "data\\TEXTURE\\explosion001.png"
#define TEXTURE_NAME_ENE "data\\TEXTURE\\gi.jpg"
#define TEXTURE_NAME_ENE2 "data\\TEXTURE\\anko.png"
#define TEXTURE_NAME_ENE3 "data\\TEXTURE\\anko2.png"
#define TEXTURE_NAME_ENE4 "data\\TEXTURE\\cat.png"
#define TEXTURE_SCORE "data\\TEXTURE\\number000.png"
#define TEXTURE_SCORE2 "data\\TEXTURE\\number001.png"
#define TEXTURE_SELECTBACK "data\\TEXTURE\\Back to Title.png"
#define TEXTURE_SELECTRETRY "data\\TEXTURE\\Retry.png"
#define TEXTURE_SELECTEXIT "data\\TEXTURE\\Exit.png"
#define TEXTURE_EFFECT "data\\TEXTURE\\effect000.jpg"

// 大きさ
#define SCREEN_WIDTH (1280) // 幅
#define SCREEN_HEIGHT (720) // 高さ

// 頂点
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // 頂点フォーマット
#define VT_MAX (4)
#define UV_DEF (1.0f) // テクスチャのデフォルトのサイズ

// コントローラー
#define	ELE_CON "GP30X-Dinput"
#define PS_CON "Wireless Controller"
#define NIN_CON "Pro Controller"

// 頂点情報の構造体の定義
typedef struct
{
	D3DXVECTOR3 pos; // 座標
	float rhw; // 座標変換係数(1.0f)
	D3DCOLOR col; // カラー
	D3DXVECTOR2 tex; // テクスチャ座標
} VERTEX_2D;

// 画面の種類
typedef enum
{
	MODE_TITLE = 0,
	MODE_DEMO,
	MODE_TUTO,
	MODE_GAME,
	MODE_VS,
	MODE_RESULT,
	MODE_RANK,
	MODE_OPTION,
	MODE_MAX
} MODE;

// プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // ウインドウプロシージャ
HRESULT Init(HINSTANCE hInstanse, HWND hWnd, BOOL bWindow = TRUE); // 初期化処理
void Uninit(void); // 終了処理
void Update(void); // 更新処理
void Draw(void); // 描画処理
LPDIRECT3DDEVICE9 GetDevice(void); // デバイスの取得
void SetMode(MODE mode);
MODE GetMode(void);
HWND GethWnd(void);
HINSTANCE GethInstanse(void);

void ConfineCursorToWindow(HWND hwnd);
void ReleaseCursor(void);

void ToggleFullScreen(HWND hWnd);
void ResetDevice(void);

#endif // _MAIN_H_