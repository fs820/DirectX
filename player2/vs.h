//------------------------------------------
//
//‘ÎíƒƒCƒ“‚Ìˆ—‚Ì’è‹`EéŒ¾[vs.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _VS_H_
#define _VS_H_

#include"main.h"

#define INTER (60)

typedef enum
{
	VSSTATE_NONE = 0,
	VSSTATE_NORMAL,
	VSSTATE_PAUSE,
	VSSTATE_END,
	VSSTATE_MAX
}VSSTATE;

void InitVs(void);
void UninitVs(void);
void UpdateVs(void);
void DrawVs(void);
void SetVsState(VSSTATE state);
VSSTATE GetVsState(void);

#endif _VS_H_
