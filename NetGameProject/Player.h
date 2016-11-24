#pragma once
#include <windows.h>
#include "stdafx.h"

extern int map[MAP_CNUM][MAP_CNUM];

class Player
{
private:
	// �÷��̾� �簢���� ����
	int pnum;			// �÷��̾� ��ȣ
	int x, y;			// ��ǥ
	int dir;			// ����
	int buf_state;		// ���� ����
	int score;			// ����
	int move_speed;		// �ӵ�
	bool type;			// true �̸� ����, false �̸� �Ϲ�
	unsigned long buf_start_time;		// ������ ���۵� �ð�


public:
	Player();
	Player(int num);
	~Player();

	void Init(bool input_type);				// �÷��̾� �ʱ�ȭ
	void TimeOver_Reset();					// �ð��� ���� ��� �÷��̾� �ʱ�ȭ
	void Move();							// �÷��̾ �����δ�
	void Buf_Check();						// ���� ���¸� üũ�Ͽ� ���ӽð��� ������ ����
	void Key_Event();						// Ű�Է� �̺�Ʈ�� �÷��̾��� ������ ����
	void Draw(HDC mem1dc);					// �÷��̾ �׸�
	void PrintScore(HDC mem1dc);			// ���ھ� ���
	void Update();							// �÷��̾ ������Ʈ

	pdata_t Get_Player_Info();				// �÷��̾��� ������ �����Ͽ� �����´�
	void Set_Player_Info(pdata_t inval);	// �÷��̾��� ������ �����Ѵ�
};

