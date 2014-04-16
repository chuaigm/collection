#pragma once

#include "GlobalData.h"

class CPlayer
{
public:
	CPlayer(void);
	~CPlayer(void);

public:
	float x;
	float y;
	float R;
	float speed;
	int   texture;

	bool  b_excl;

	void InitPlayer( float px, float py, float pr, float pspeed );
	void DrawPlayer();
	void DrawPlayer(float px, float py, float pr, int pt);
	void DrawExcellent(float px, float py, DWORD inc);
};
