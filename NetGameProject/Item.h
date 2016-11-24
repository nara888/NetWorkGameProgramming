#pragma once
#include <windows.h>
#include "stdafx.h"

class Item
{
private:
	// 아이템
	int x, y;
	int item_type;

public:
	Item();
	~Item();
	void Init();
	void Draw(HDC mem1dc);
	void Update();
	void Remove();

	idata_t Get_Item_Info();				// 아이템의 정보를 리턴하여 가져온다
	void Set_Item_Info(idata_t inval);		// 아이템의 정보를 셋팅한다
};

