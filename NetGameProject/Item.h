#pragma once
#include <windows.h>
#include "stdafx.h"

class Item
{
private:
	// ������
	int x, y;
	int item_type;

public:
	Item();
	~Item();
	void Init();
	void Draw(HDC mem1dc);
	void Update();
	void Remove();

	idata_t Get_Item_Info();				// �������� ������ �����Ͽ� �����´�
	void Set_Item_Info(idata_t inval);		// �������� ������ �����Ѵ�
};

