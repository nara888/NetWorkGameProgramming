#pragma once
#include "stdafx.h"
#include "Player.h"
#include "Item.h"

class Collision
{
public:
	Collision();
	~Collision();

	void Player_to_Wall_Collision(Player* p);
	void Player_to_Potal_Collision(Player* p);
	BOOL Player_to_Player_Collision(Player* p1, Player* p2);
	void Player_to_Item_Collision(Player* p, Item* it);

	BOOL Rect_Check_Collision(int x1, int y1, int size1, int x2, int y2, int size2);
};

