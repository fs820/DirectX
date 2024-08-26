//---------------------------------------
//
//入力処理の定義・宣言[input.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _INPUT_H_
#define _INPUT_H_
#include"main.h"

#define STICK_DED (0.8f)
#define VIBRATION_MAX (65535)

//POV
#define POV_UP (0.0f)
#define POV_RIGHTUP (4500.0f)
#define POV_RIGHT (9000.0f)
#define POV_RIGHTDOWN (13500.0f)
#define POV_DOWN (18000.0f)
#define POV_LEFTDOWN (22500.0f)
#define POV_LEFT (27000.0f)
#define POV_LEFTUP (31500.0f)

//マウスボタンの種類
typedef enum
{
	MOUSE_LEFT = 0,
	MOUSE_RIGHT,
	MOUSE_SENTER,
	MOUSE_B1,
	MOUSE_B2,
	MOUSE_B3,
	MOUSE_B4,
	MOUSE_B5,
	MOUSE_MAX
}MOUSE;

typedef enum
{
	CONTROLLER_1 = 0,
	CONTROLLER_2,
	CONTROLLER_3,
	CONTROLLER_4,
	CONTROLLER_MAX,
	CONTROLLER_MAX2
}CONTROLLER;

typedef enum
{
	CONTYPE_X = 0,
	CONTYPE_D,
	CONTYPE_MAX
}CONTYPE;

//パッドボタンの種類
typedef enum
{
	JOYKEY_UP = 0,
	JOYKEY_DOWN,
	JOYKEY_LEFT,
	JOYKEY_RIGHT,
	JOYKEY_START,
	JOYKEY_BACK,
	JOYKEY_L3,
	JOYKEY_R3,
	JOYKEY_LB,
	JOYKEY_RB,
	JOYKEY_LT,
	JOYKEY_RT,
	JOYKEY_A,
	JOYKEY_B,
	JOYKEY_X,
	JOYKEY_Y,
	JOYKEY_MAX
}JOYKEY;

//パッドボタンの種類
typedef enum
{
	ELEKEY_A = 0,
	ELEKEY_B,
	ELEKEY_X,
	ELEKEY_Y,
	ELEKEY_LB,
	ELEKEY_RB,
	ELEKEY_LT,
	ELEKEY_RT,
	ELEKEY_L3,
	ELEKEY_R3,
	ELEKEY_BACK,
	ELEKEY_START,
	ELEKEY_SENTER,
	ELEKEY_MAX
}ELEKEY;

//パッドボタンの種類
typedef enum
{
	PSKEY_□ = 0,
	PSKEY_×,
	PSKEY_○,
	PSKEY_△,
	PSKEY_LB,
	PSKEY_RB,
	PSKEY_LT,
	PSKEY_RT,
	PSKEY_BACK,
	PSKEY_START,
	PSKEY_L3,
	PSKEY_R3,
	PSKEY_SENTER,
	PSKEY_PAD,
	PSKEY_MAX
}PSKEY;

//パッドボタンの種類
typedef enum
{
	NINKEY_B = 0,
	NINKEY_A,
	NINKEY_Y,
	NINKEY_X,
	NINKEY_LB,
	NINKEY_RB,
	NINKEY_LT,
	NINKEY_RT,
	NINKEY_－,
	NINKEY_＋,
	NINKEY_L3,
	NINKEY_R3,
	NINKEY_HOME,
	NINKEY_CAP,
	NINKEY_MAX
}NINKEY;

//パッドボタンの種類
typedef enum
{
	DKEY_A = 0,
	DKEY_B,
	DKEY_X,
	DKEY_Y,
	DKEY_LB,
	DKEY_RB,
	DKEY_LT,
	DKEY_RT,
	DKEY_L3,
	DKEY_R3,
	DKEY_BACK,
	DKEY_START,
	DKEY_SENTER,
	DKEY_B1,
	DKEY_B2,
	DKEY_MAX
}DKEY;

//パッドスティックの種類
typedef enum
{
	STICK_LEFT = 0,
	STICK_RIGHT,
	STICK_MAX
}STICK;

typedef enum
{
	DIRESTICK_UP = 0,
	DIRESTICK_DOWN,
	DIRESTICK_LEFT,
	DIRESTICK_RIGHT,
	DIRESTICK_MAX
}DIRESTICK;

//キーボード
HRESULT InitKeyboard(HINSTANCE hInstanse, HWND hWnd);//初期化処理
void UninitKeyborad(void);//終了処理
void UpdateKeyborad(void);//更新処理
bool GetKeyboradPress(int nkey);//キー押下処理
bool GetKeyboradTrigger(int nkey);//キー押下処理
bool GetKeyboradRelease(int nkey);//キー押下処理
bool GetKeyboradRepeat(int nkey);//キー押下処理

//マウス
HRESULT InitMouse(HINSTANCE hInstanse, HWND hWnd);//初期化処理
void UninitMouse(void);//終了処理
void UpdateMouse(void);//更新処理
bool GetMousePress(int nkey);//キー押下処理
bool GetMouseTrigger(int nkey);//キー押下処理
bool GetMouseRelease(int nkey);//キー押下処理
bool GetMouseRepeat(int nkey);//キー押下処理
float *GetMouseMove(void);//キー押下処理
float GetMouseWheel(void);//キー押下処理

//Xinput
HRESULT InitJoypad(void);//初期化処理
void UninitJoypad(void);//終了処理
void UpdateJoypad(void);//更新処理
bool GetJoykeyPress(JOYKEY key, CONTROLLER Controller);//キー押下処理
bool GetJoykeyTrigger(JOYKEY key, CONTROLLER Controller);//キー押下処理
bool GetJoykeyRelease(JOYKEY key, CONTROLLER Controller);//キー押下処理
bool GetJoykeyRepeat(JOYKEY key, CONTROLLER Controller);//キー押下処理
float *GetJoyStick(STICK Stick, CONTROLLER Controller);//スティック処理
bool JoyStickPress(DIRESTICK DireStick ,STICK Stick, CONTROLLER Controller);//スティック処理
bool JoyStickTrigger(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//スティック処理
bool JoyStickRelease(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//スティック処理
bool JoyStickRepeat(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//スティック処理
void VibrateController(WORD wLeftMotorSpeed, WORD wRightMotorSpeed, CONTROLLER Controller);

//dinputパッド
HRESULT InitdJoypad(HINSTANCE hInstanse, HWND hWnd);//初期化処理
void UninitdJoypad(void);//終了処理
void UpdatedJoypad(void);//更新処理
bool GetdJoykeyPress(int nkey, CONTROLLER Controller);//キー押下処理
bool GetdJoykeyTrigger(int nkey, CONTROLLER Controller);//キー押下処理
bool GetdJoykeyRelease(int nkey, CONTROLLER Controller);//キー押下処理
bool GetdJoykeyRepeat(int nkey, CONTROLLER Controller);//キー押下処理
float* GetdJoyStick(STICK Stick, CONTROLLER Controller);//スティック処理
bool dJoyStickPress(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//スティック処理
bool dJoyStickTrigger(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//スティック処理
bool dJoyStickRelease(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//スティック処理
bool dJoyStickRepeat(DIRESTICK DireStick, STICK Stick, CONTROLLER Controller);//スティック処理
float GetdJoySlider(int nSlider, CONTROLLER Controller);
bool GetdJoyPov(float fData,int nPov, CONTROLLER Controller);
bool GetdJoyPovTrigger(float fData, int nPov, CONTROLLER Controller);
bool GetdJoyPovRelease(float fData, int nPov, CONTROLLER Controller);
bool GetdJoyPovRepeat(float fData, int nPov, CONTROLLER Controller);
void VibratedController(LONG lMotorPower, CONTROLLER Controller);
void VibratedControllerStop(CONTROLLER Controller);

//接続確認
bool IsXInputControllerConnected(CONTROLLER Controller);
bool IsDirectInputControllerConnected(CONTROLLER Controller);
char *ControllerName(CONTROLLER Controller);
int ControllerNum(CONTYPE Contype);
BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

#endif _INPUT_H_

