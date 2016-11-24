#pragma once
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
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

#define END_RESET_COUNT 5		// 몇번 리셋되면 게임이 끝나는가

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
#define MAP_SPACE		100      //맵 여백을 위함
#define PLAYER_TAGGER	0
#define PLAYER_RUNNER	1

#define BUF_NONE		1
#define BUF_SPEED		2

#define TIME_DURATION_BUF_SPEED	5000	// 스피드업 아이템 지속시간

#define ITEM_SIZE		20
#define ITEM_SCORE		1
#define ITEM_SPEED_UP	2

#define GET_TOUCH_SCORE	50
#define GET_ITEM_SCORE	30
#define GET_TIMEOUT_SCORE 50

#define CHANGE_TIME		60000	// 술래와 일반 플레이어가 바뀌는 시간 ms단위

#define SERVERIP   "127.0.0.1"
#define SERVERPORT	9000


typedef struct PlayerData {
	// 플레이어의 정보
	int pnum;			// 플레이어 번호
	int x, y;			// 좌표
	int dir;			// 방향
	int move_speed;		// 속도
	int buf_state;		// 버프 상태
	int score;			// 점수
	bool type;			// true 이면 술래, false 이면 일반
	unsigned long buf_start_time;	// 버프 시작 시간
}pdata_t;

typedef struct ItemData {
	// 아이템의 정보
	int x, y;			// 좌표
	int item_type;		// 아이템 타입
}idata_t;
// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
