#include "Item.h"
#include "Player.h"



Item::Item()
{
}


Item::~Item()
{
}

void Item::Init()
{
	int col, row;
	col = rand() % 19;
	row = rand() % 19;
	item_type = rand() % 2 + 1;	// 스피드업 아이템, 스코어 아이템

	while (map[row][col] == 1)	// 해당 위치가 벽이면 다시 랜덤을 돌린다
	{
		col = rand() % 19;
		row = rand() % 19;
	}
	x = col * MAP_SIZE + 5;
	y = row * MAP_SIZE + MAP_SPACE + 5;
}

void Item::Update()
{

}

void Item::Draw(HDC mem1dc)
{
	HPEN hPen, oldPen;

	if (item_type == ITEM_SCORE)
	{
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		oldPen = (HPEN)SelectObject(mem1dc, hPen);
		Ellipse(mem1dc, x, y, x + ITEM_SIZE, y + ITEM_SIZE); // 아이템 그리기
		SelectObject(mem1dc, oldPen);
		DeleteObject(hPen);
	}
	else if (item_type == ITEM_SPEED_UP)
	{
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		oldPen = (HPEN)SelectObject(mem1dc, hPen);
		Ellipse(mem1dc, x, y, x + ITEM_SIZE, y + ITEM_SIZE); // 아이템 그리기
		SelectObject(mem1dc, oldPen);
		DeleteObject(hPen);
	}
	
}

void Item::Remove()
{

}

idata_t Item::Get_Item_Info()
{
	idata_t retval;

	retval.x = x;
	retval.y = y;
	retval.item_type = item_type;
	
	return retval;
}

void Item::Set_Item_Info(idata_t inval)
{
	x = inval.x;
	y = inval.y;
	item_type = inval.item_type;
}