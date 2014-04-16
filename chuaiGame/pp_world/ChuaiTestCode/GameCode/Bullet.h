#pragma once

#include "GlobalData.h"

class CBullet
{
public:
	CBullet(void);
	~CBullet(void);

public:
	float x;
	float y;
	float R;
	float speed;
	int   attrib;
	int   texture;
	// ��������
	float		v_x;
	float		v_y;

	void InitBullet(float bx, float by);
	void DrawBullet();
	void DrawBullet(float bx, float by, float br, int bt);
};
