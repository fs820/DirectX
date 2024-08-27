//------------------------------------------
//
//オプションの処理[option.cpp]
//Author fuma sato
//
//------------------------------------------

#include"option.h"
#include"input.h"
#include"fade.h"
#include"sound.h"

void InitOption(void)
{

}


void UninitOption(void)
{

}


void UpdateOption(void)
{
	if (GetKeyboradTrigger(DIK_BACK) == true || GetJoykeyTrigger(JOYKEY_BACK,CONTROLLER_MAX) == true || GetMouseTrigger(MOUSE_RIGHT) == true)
	{
		FADE fade;
		fade = GetFade();
		if (fade == FADE_NONE)
		{
			//サウンド
			StopSound();
			//切替
			SetFade(MODE_TITLE);
		}
	}

	for (int i = 0; i < ControllerNum(CONTYPE_D); i++)
	{
		if (!strcmp(ControllerName((CONTROLLER)i), ELE_CON))
		{
			if (GetdJoykeyTrigger(ELEKEY_BACK,(CONTROLLER)i))
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_TITLE);
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), PS_CON))
		{
			if (GetdJoykeyTrigger(PSKEY_BACK, (CONTROLLER)i))
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_TITLE);
				}
			}
		}
		else if (!strcmp(ControllerName((CONTROLLER)i), NIN_CON))
		{
			if (GetdJoykeyTrigger(NINKEY_－, (CONTROLLER)i))
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_TITLE);
				}
			}
		}
		else if (!IsXInputControllerConnected((CONTROLLER)i) && IsDirectInputControllerConnected((CONTROLLER)i))
		{
			if (GetdJoykeyTrigger(DKEY_BACK, (CONTROLLER)i))
			{
				FADE fade;
				fade = GetFade();
				if (fade == FADE_NONE)
				{
					//サウンド
					StopSound();
					//切替
					SetFade(MODE_TITLE);
				}
			}
		}
	}
}


void DrawOption(void)
{

}