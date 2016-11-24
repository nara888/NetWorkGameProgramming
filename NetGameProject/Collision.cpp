#include "Collision.h"



Collision::Collision()
{
}


Collision::~Collision()
{
}


BOOL Collision::Player_to_Player_Collision(Player* p1, Player* p2)
{
	// P1�� P2�� ������ �����´�
	pdata_t pdata1 = p1->Get_Player_Info();
	pdata_t pdata2 = p2->Get_Player_Info();


	// �� �÷��̾ �浹�Ѵٸ�
	if (Rect_Check_Collision(pdata1.x, pdata1.y, PLAYER_SIZE, pdata2.x, pdata2.y, PLAYER_SIZE))
	{
		if (pdata1.type)		// P1�� �����̸�
		{
			pdata1.score += GET_TOUCH_SCORE;	// P1 ��ġ���ھ ����
			pdata1.type = FALSE;				// P1 �Ϲ� �÷��̾�� ����
			p1->Set_Player_Info(pdata1);		// P1 ���� ����
			p2->Init(TRUE);						// P2�� ������ �ٲٰ� ���ο� ��ġ�� �ʱ�ȭ
		}
		else if (pdata2.type)	// P2�� �����̸�
		{
			pdata2.score += GET_TOUCH_SCORE;	// P2 ��ġ���ھ ����
			pdata2.type = FALSE;				// P2 �Ϲ� �÷��̾�� ����
			p2->Set_Player_Info(pdata2);		// P2 ���� ����
			p1->Init(TRUE);						// P1�� ������ �ٲٰ� ���ο� ��ġ�� �ʱ�ȭ
		}
		return TRUE;
	}
	else
		return FALSE;
}

void Collision::Player_to_Wall_Collision(Player* p)
{
	// �÷��̾� ������ �����´�
	pdata_t pdata = p->Get_Player_Info();
	int check_x, check_y, cnt = 0;

	for (int i = 0; i < MAP_CNUM; ++i) {
		for (int j = 0; j < MAP_CNUM; ++j) {
			if (map[i][j] == 1 && Rect_Check_Collision(pdata.x, pdata.y, PLAYER_SIZE, j*MAP_SIZE, i*MAP_SIZE + MAP_SPACE, MAP_SIZE))
			{
				// ���� ��� �΋H�ƴ°�
				cnt++;
				// ���� LEFT
				check_x = j*MAP_SIZE;
				// ���� TOP
				check_y = i*MAP_SIZE + MAP_SPACE;
			}
		}
	}

	// �΋Hģ ���� 2���� ���
	if (cnt == 2)
	{
		// �÷��̾��� ���⿡ ���� ��ǥ���� ����
		if (pdata.dir == DIR_RIGHT)
			pdata.x -= pdata.move_speed;
		else if (pdata.dir == DIR_LEFT)
			pdata.x += pdata.move_speed;
		else if (pdata.dir == DIR_UP)
			pdata.y += pdata.move_speed;
		else if (pdata.dir == DIR_DOWN)
			pdata.y -= pdata.move_speed;
	}

	// �΋Hģ ���� 1���� ���
	else if (cnt == 1)
	{
		if (pdata.dir == DIR_RIGHT)
		{
			// ���� ��Ȯ�� ���ļ� �΋Hġ�� �ݹ߷��� �ش�
			if (pdata.y >= check_y && pdata.y + PLAYER_SIZE <= check_y + MAP_SIZE)
				pdata.x -= pdata.move_speed;
			else
			{
				// �÷��̾ ������ �Ʒ��̸� y���� �����ش�
				if (pdata.y >= check_y)
					pdata.y += pdata.move_speed;
				// �÷��̾ ������ ���̸� y���� ���ش�
				else if (pdata.y < check_y)
					pdata.y -= pdata.move_speed;
				// ���� ���� �ݹ߷��� �ش�
				pdata.x -= pdata.move_speed;
			}
		}

		else if (pdata.dir == DIR_LEFT)
		{
			if (pdata.y >= check_y && pdata.y + PLAYER_SIZE <= check_y + MAP_SIZE)
				pdata.x += pdata.move_speed;
			else {
				if (pdata.y >= check_y)
					pdata.y += pdata.move_speed;
				else if (pdata.y < check_y)
					pdata.y -= pdata.move_speed;
				pdata.x += pdata.move_speed;
			}
		}

		else if (pdata.dir == DIR_UP)
		{
			if (pdata.x >= check_x && pdata.x + PLAYER_SIZE <= check_x + MAP_SIZE)
				pdata.y += pdata.move_speed;
			else 
			{
				if (pdata.x >= check_x)
					pdata.x += pdata.move_speed;
				else if (pdata.x < check_x)
					pdata.x -= pdata.move_speed;
				pdata.y += pdata.move_speed;
			}
		}

		else if (pdata.dir == DIR_DOWN)
		{
			if (pdata.x >= check_x && pdata.x + PLAYER_SIZE <= check_x + MAP_SIZE)
				pdata.y -= pdata.move_speed;
			else 
			{
				if (pdata.x >= check_x)
					pdata.x += pdata.move_speed;
				else if (pdata.x < check_x)
					pdata.x -= pdata.move_speed;
				pdata.y -= pdata.move_speed;
			}
		}
	}

	p->Set_Player_Info(pdata);
}

void Collision::Player_to_Item_Collision(Player* p, Item* it)
{
	// �÷��̾� ������ �����´�
	pdata_t pdata = p->Get_Player_Info();
	// ������ ������ �����´�
	idata_t idata = it->Get_Item_Info();

	if (Rect_Check_Collision(pdata.x, pdata.y, PLAYER_SIZE, idata.x, idata.y, ITEM_SIZE))
	{
		it->Init();
		if (idata.item_type == ITEM_SCORE)
		{
			pdata.score += GET_ITEM_SCORE;
		}
		else if (idata.item_type == ITEM_SPEED_UP)
		{
			pdata.move_speed = BUF_MOVE_SPEED;
			pdata.buf_state = BUF_SPEED;
			pdata.buf_start_time = GetTickCount();
		}
		p->Set_Player_Info(pdata);
	}
}

void Collision::Player_to_Potal_Collision(Player* p)
{
	// �÷��̾� ������ �����´�
	pdata_t pdata = p->Get_Player_Info();

	if (Rect_Check_Collision(pdata.x, pdata.y, PLAYER_SIZE, -1 * MAP_SIZE, 8 * MAP_SIZE + MAP_SPACE, MAP_SIZE))
	{
		pdata.x = 18 * MAP_SIZE + 1;
	}
	else if (Rect_Check_Collision(pdata.x, pdata.y, PLAYER_SIZE, 19 * MAP_SIZE, 8 * MAP_SIZE + MAP_SPACE, MAP_SIZE))
	{
		pdata.x = 1;
	}
	p->Set_Player_Info(pdata);
}

BOOL Collision::Rect_Check_Collision(int x1, int y1, int size1, int x2, int y2, int size2)
{
	RECT r, temp1, temp2;

	temp1.left = x1;
	temp1.right = x1 + size1;
	temp1.top = y1;
	temp1.bottom = y1 + size1;

	temp2.left = x2;
	temp2.right = x2 + size2;
	temp2.top = y2;
	temp2.bottom = y2 + size2;

	if (IntersectRect(&r, &temp1, &temp2))
		return TRUE;
	else
		return FALSE;
}