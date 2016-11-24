#include "Player.h"


int map[MAP_CNUM][MAP_CNUM];

Player::Player(int num)
{
	score = 0;
	pnum = num;
}


Player::~Player()
{
}

// input_type �� TRUE�̸� ����, FALSE�̸� �Ϲ�
void Player::Init(bool input_type)
{
	int col, row;
	col = rand() % 19;
	row = rand() % 19;

	while (map[row][col] == 1)	// �ش� ��ġ�� ���̸� �ٽ� ������ ������
	{
		col = rand() % 19;
		row = rand() % 19;
	}

	// �÷��̾��� ��ǥ�� Ÿ��, ������ �ʱ�ȭ�Ѵ�
	x = col * MAP_SIZE + 1;
	y = row * MAP_SIZE + MAP_SPACE + 1;
	buf_state = BUF_NONE;
	move_speed = DEFAULT_MOVE_SPEED;
	type = input_type;
	dir = DIR_NONE;
}

void Player::TimeOver_Reset()
{
	if (type)
		Init(FALSE);
	else
	{
		score += GET_TIMEOUT_SCORE;
		Init(TRUE);
	}
}

void Player::Move()
{
	if (dir == DIR_RIGHT)
		x += move_speed;
	else if (dir == DIR_LEFT)
		x -= move_speed;
	else if (dir == DIR_UP)
		y -= move_speed;
	else if (dir == DIR_DOWN)
		y += move_speed;
}

void Player::Buf_Check()
{
	if (buf_state == BUF_NONE)
		return;
	else if(buf_state == BUF_SPEED)
	{
		if (GetTickCount() - buf_start_time > TIME_DURATION_BUF_SPEED)
		{
			move_speed = DEFAULT_MOVE_SPEED;
			buf_state = BUF_NONE;
			buf_start_time = 0;
		}
	}
}

void Player::Key_Event()
{
	// ����Ű�� �÷��̾� ������ȯ

	if (GetAsyncKeyState(VK_LEFT) && 0x8000)
		dir = DIR_LEFT;
	else if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
		dir = DIR_RIGHT;
	else if (GetAsyncKeyState(VK_UP) && 0x8000)
		dir = DIR_UP;
	else if (GetAsyncKeyState(VK_DOWN) && 0x8000)
		dir = DIR_DOWN;

}

void Player::Draw(HDC mem1dc)
{
	HPEN hPen, oldPen;

	// ���� �÷��̾��� ��� �β��� ������
	if (type)
	{
		hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(mem1dc, hPen);
		Rectangle(mem1dc, x, y, x + PLAYER_SIZE, y + PLAYER_SIZE);
		SelectObject(mem1dc, oldPen);
		DeleteObject(hPen);
	}

	// �Ϲ� �÷��̾��� ��� �Ķ���
	else
	{
		hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
		oldPen = (HPEN)SelectObject(mem1dc, hPen);
		Rectangle(mem1dc, x, y, x + PLAYER_SIZE, y + PLAYER_SIZE);
		SelectObject(mem1dc, oldPen);
		DeleteObject(hPen);
	}

	// ���ھ� ���
	PrintScore(mem1dc);
	
}

void Player::PrintScore(HDC mem1dc)
{
	char str[20];

	wsprintf(str, "P%d SCORE : %d", pnum, score);
	TextOut(mem1dc, 400, 20 * pnum, str, strlen(str));
}

pdata_t Player::Get_Player_Info()
{
	pdata_t retval;

	retval.pnum = pnum;
	retval.x = x;
	retval.y = y;
	retval.type = type;
	retval.dir = dir;
	retval.move_speed = move_speed;
	retval.buf_state = buf_state;
	retval.score = score;
	retval.buf_start_time = buf_start_time;

	return retval;
}

void Player::Set_Player_Info(pdata_t inval)
{
	pnum = inval.pnum;
	x = inval.x;
	y = inval.y;
	type = inval.type;
	dir = inval.dir;
	move_speed = inval.move_speed;
	buf_state = inval.buf_state;
	score = inval.score;
	buf_start_time = inval.buf_start_time;
}


void Player::Update()
{
	Buf_Check();
	Move();
}