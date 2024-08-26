//------------------------------------------
//
//チュートリアルの定義・宣言[tuto.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _TUTO_H_
#define _TUTO_H_

#include"game.h"

void InitTuto(void);
void UninitTuto(void);
void UpdateTuto(void);
void DrawTuto(void);
void SetTutoState(GAMESTATE state);
GAMESTATE GetTutoState(void);

#endif _TUTO_H_