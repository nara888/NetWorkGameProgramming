#pragma once
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once
#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <math.h>
#include <chrono>
#include <iostream>
#include <time.h>


#define PLAYER_NUM		2
#define PLAYER_SIZE		27

#define END_RESET_COUNT 5		// ��� ���µǸ� ������ �����°�

#define DIR_NONE		0
#define DIR_UP			1
#define DIR_RIGHT		2
#define DIR_DOWN		3
#define DIR_LEFT		4
#define DEFAULT_MOVE_SPEED		3
#define BUF_MOVE_SPEED			5
#define GAME_SPEED		30
#define MAP_SIZE		30
#define MAP_CNUM		19
#define MAP_SPACE		100      //�� ������ ����
#define PLAYER_TAGGER	0
#define PLAYER_RUNNER	1

#define BUF_NONE		1
#define BUF_SPEED		2

#define TIME_DURATION_BUF_SPEED	5000	// ���ǵ�� ������ ���ӽð�

#define ITEM_SIZE		20
#define ITEM_SCORE		1
#define ITEM_SPEED_UP	2

#define GET_TOUCH_SCORE	50
#define GET_ITEM_SCORE	30
#define GET_TIMEOUT_SCORE 50

#define CHANGE_TIME		60000	// ������ �Ϲ� �÷��̾ �ٲ�� �ð� ms����

#define SERVERIP   "127.0.0.1"
#define SERVERPORT	9000


typedef struct PlayerData {
	// �÷��̾��� ����
	int pnum;			// �÷��̾� ��ȣ
	int x, y;			// ��ǥ
	int dir;			// ����
	int move_speed;		// �ӵ�
	int buf_state;		// ���� ����
	int score;			// ����
	bool type;			// true �̸� ����, false �̸� �Ϲ�
	unsigned long buf_start_time;	// ���� ���� �ð�
}pdata_t;

typedef struct ItemData {
	// �������� ����
	int x, y;			// ��ǥ
	int item_type;		// ������ Ÿ��
}idata_t;
// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
