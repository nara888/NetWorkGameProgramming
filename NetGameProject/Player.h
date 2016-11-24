#pragma once
#include <windows.h>
#include "stdafx.h"

extern int map[MAP_CNUM][MAP_CNUM];

class Player
{
private:
	// 플레이어 사각형의 정보
	int pnum;			// 플레이어 번호
	int x, y;			// 좌표
	int dir;			// 방향
	int buf_state;		// 버프 상태
	int score;			// 점수
	int move_speed;		// 속도
	bool type;			// true 이면 술래, false 이면 일반
	unsigned long buf_start_time;		// 버프가 시작된 시간


public:
	Player();
	Player(int num);
	~Player();

	void Init(bool input_type);				// 플레이어 초기화
	void TimeOver_Reset();					// 시간이 지날 경우 플레이어 초기화
	void Move();							// 플레이어를 움직인다
	void Buf_Check();						// 버프 상태를 체크하여 지속시간이 지나면 해제
	void Key_Event();						// 키입력 이벤트로 플레이어의 방향을 설정
	void Draw(HDC mem1dc);					// 플레이어를 그림
	void PrintScore(HDC mem1dc);			// 스코어 출력
	void Update();							// 플레이어를 업데이트

	pdata_t Get_Player_Info();				// 플레이어의 정보를 리턴하여 가져온다
	void Set_Player_Info(pdata_t inval);	// 플레이어의 정보를 셋팅한다
};

