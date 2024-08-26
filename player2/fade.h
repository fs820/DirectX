//------------------------------------------
//
//フェード処理の定義・宣言[fade.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _FADE_H_
#define _FADE_H_

#include"main.h"

//フェードの状態
typedef enum
{
	FADE_NONE=0,
	FADE_IN,
	FADE_OUT,
	FADE_MAX
}FADE;

void InitFade(MODE modeNext);//初期化処理
void UninitFade(void);//終了処理
void UpdateFade(void);//更新処理
void DrawFade(void);//描画処理
void SetFade(MODE modeNext);
FADE GetFade(void);

#endif  _FADE_H_

