#include <windows.h> // 윈도우 헤더 파일
#include <stdio.h>
#include <stdlib.h>
#include "resource.h"
#include "stdafx.h"
#include "Player.h"
#include "Item.h"
#include "Collision.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Hide_and_Seek";
LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam);

void ReadMapData();																// 텍스트파일을 읽어서 맵을 로드(길 0  벽 1)
void DrawMap(HDC mem1dc, HDC mem2dc, HBITMAP wallBit);							// 맵 출력
void DrawBackground(HDC mem1dc, HDC mem2dc, RECT winRect, HBITMAP backBit);		// 백그라운드 출력
void Print_Time(HDC mem1dc, int time);											// 경과 시간 출력
void Print_Reset_Count(HDC mem1dc, int cnt);									// 리셋 카운트 출력
void Print_End_Message(HDC mem1dc, Player *p1, Player *p2);						// 종료 메세지 출력


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE
	hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground =
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszClass,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		585, 710,
		NULL, (HMENU)NULL,
		hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, mem1dc, mem2dc;
	BITMAP bmp;
	static bool game_state = TRUE;				// TRUE : 게임중, FALSE : 게임종료
	static int game_time = GAME_SPEED;			// 업데이트 타이머 속도 (ms)
	static int reset_cnt = 0;					// 리셋 된 횟수
	static unsigned long start_time, end_time;	// 게임이 경과한 시간
	static RECT winRect;						// 측정하려는 윈도우 핸들과 크기
	static HBITMAP hBit1, oldBit1, oldBit2;		// 더블 버퍼링 비트맵
	static HBITMAP backBit;						// 백그라운드 비트맵
	static HBITMAP wallBit;						// 벽 비트맵
	static Player *p1, *p2;						// 플레이어 1, 2
	static Item* it1;							// 아이템1
	static Collision* Col;						// 충돌체크 클래스

	switch (iMessage) {
	case WM_CREATE:
		srand(time(NULL));
		ReadMapData();   // 맵을 그리기위한 데이터 읽어옴
		GetClientRect(hWnd, &winRect); // 윈도우 경계좌표, 윈도우 크기 측정해 winRect에다 저장

		backBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1)); // 백그라운드 이미지 로드
		wallBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALL_BIT)); // 벽 이미지 로드

		// p1과 p2 생성, 플레이어 번호를 인자값으로 넣음
		p1 = new Player(1);
		p2 = new Player(2);

		// 두 플레이어중 1명을 랜덤으로 술래로 정하여 초기화한다
		if (rand() % 2 == 0)
		{	
			// P1 술래, P2 일반
			p1->Init(TRUE);
			p2->Init(FALSE);
		}
		else
		{
			// P1 일반, P2 술래
			p1->Init(FALSE);
			p2->Init(TRUE);
		}

		// 아이템 객체 생성
		it1 = new Item;
		it1->Init();

		// 충돌체크 객체 생성
		Col = new Collision;

		// 시작시간 체크
		start_time = GetTickCount();

		// 타이머 생성
		SetTimer(hWnd, 1, 50, NULL);		// 더블 버퍼링 타이머
		SetTimer(hWnd, 2, game_time, NULL); // 2번 타이머에 time만큼 시간 간격을 설정, time 시간마다 WM_TIMER 발생 

		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		mem1dc = CreateCompatibleDC(hdc);
		// hBit1에는 배경과 타일이 출력된 비트맵이 저장, mem1dc에 설정
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);

		GetObject(hBit1, sizeof(BITMAP), &bmp); // 크기

		// mem1dc에 있는 내용을 hdc에 뿌려준다.
		StretchBlt(hdc, 0, 0, winRect.right, winRect.bottom, mem1dc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);


		SelectObject(mem1dc, oldBit1);
		DeleteDC(mem1dc);

		EndPaint(hWnd, &ps);
		break;
	/*
	case WM_CHAR:
		hdc = GetDC(hWnd);
		

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	*/
	case WM_KEYDOWN:
		// 방향키로 P1 방향전환
		p1->Key_Event();

		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:		// 더블 버퍼링 타이머
			hdc = GetDC(hWnd);

			GetClientRect(hWnd, &winRect);

			if (hBit1 == NULL) // hBit1을 hdc와 호환되게 만들어준다.
				hBit1 = CreateCompatibleBitmap(hdc, winRect.right, winRect.bottom);

			mem1dc = CreateCompatibleDC(hdc);
			mem2dc = CreateCompatibleDC(mem1dc);

			oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);

			// 백그라운드 이미지 출력
			DrawBackground(mem1dc, mem2dc, winRect, backBit);

			// 맵 출력
			DrawMap(mem1dc, mem2dc, wallBit);

			// 플레이어 출력
			p1->Draw(mem1dc);
			p2->Draw(mem1dc);

			// 아이템 출력
			it1->Draw(mem1dc);

			// 시간 출력
			Print_Time(mem1dc, GetTickCount() - start_time);

			// 리셋 카운트 출력
			Print_Reset_Count(mem1dc, reset_cnt);

			// 게임이 종료되면 종료 메시지 출력
			if(game_state == FALSE)
				Print_End_Message(mem1dc, p1, p2);

			DeleteDC(mem2dc);
			SelectObject(mem1dc, oldBit1);
			DeleteDC(mem1dc);
			ReleaseDC(hWnd, hdc);

			break;


		case 2:		// 업데이트 타이머 (충돌체크)
			GetClientRect(hWnd, &winRect); // 윈도우 경계좌표, 윈도우 크기 측정해 winRect에다 저장

			if (END_RESET_COUNT - reset_cnt <= 0)
				game_state = FALSE;


			// 게임 진행상태
			if (game_state)
			{
				// 플레이어 업데이트
				p1->Update();
				p2->Update();

				// 플레이어와 벽의 충돌체크
				Col->Player_to_Wall_Collision(p1);
				Col->Player_to_Wall_Collision(p2);

				// 플레이어와 포탈 충돌체크
				Col->Player_to_Potal_Collision(p1);
				Col->Player_to_Potal_Collision(p2);

				// 플레이어끼리 충돌체크
				if (Col->Player_to_Player_Collision(p1, p2))
				{
					start_time = GetTickCount();
					reset_cnt++;
				}

				// 플레이어와 아이템 충돌체크
				Col->Player_to_Item_Collision(p1, it1);
				Col->Player_to_Item_Collision(p2, it1);

				// 경과 시간이 CHANGE_TIME을 지나면 플레이어를 리셋
				if (GetTickCount() - start_time > CHANGE_TIME)
				{
					p1->TimeOver_Reset();			// 플레이어들을 타임오버 리셋으로 초기화
					p2->TimeOver_Reset();
					start_time = GetTickCount();	// 시작시간 초기화
					reset_cnt++;					// 리셋 카운트를 1 더한다
				}
			}

			/*
				// 윈도우 화면과의 충돌체크
				if (player[i].x >= winRect.right - 15 || player[i].x <= winRect.left + 15)
				{
					player[i].x -= MOVE_SPEED * player[i].xdir;
					player[i].xdir = 0;
				}
				else if (player[i].y <= winRect.top + 15 || player[i].y >= winRect.bottom - 15)
				{
					player[i].y -= MOVE_SPEED * player[i].ydir;
					player[i].ydir = 0;
				}
			*/
			break;
		}

		InvalidateRect(hWnd, NULL, FALSE);
		break;


	case WM_DESTROY:
		KillTimer(hWnd, 1); // 1번 타이머의 동작 멈춤
		KillTimer(hWnd, 2); // 2번 타이머의 동작 멈춤
		delete p1, p2, it1, Col;
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam,
		lParam));
}

void ReadMapData() {
	FILE* f;
	int i = 0, j = 0;
	char c;
	f = fopen("mapdata.txt", "r");
	while ((c = getc(f)) != EOF) {
		if (c == '0' || c == '1') {
			map[i][j] = c - '0';
			++j;
			if (j == MAP_CNUM) {
				j = 0; ++i;
			}
		}
	}
	fclose(f);
}

// 맵 출력
void DrawMap(HDC mem1dc, HDC mem2dc, HBITMAP wallBit) 
{

	//BITMAP bmp;
	//HBITMAP oldBit;

	//oldBit = (HBITMAP)SelectObject(mem2dc, wallBit); // 벽 이미지를 mem2dc에 셋팅한다

	//GetObject(wallBit, sizeof(BITMAP), &bmp); // 크기
	//for (int i = 0; i < MAP_CNUM; ++i) {
	//	for (int j = 0; j < MAP_CNUM; ++j) {
	//		if (map[i][j] == 1) {
	//			StretchBlt(mem1dc, j*MAP_SIZE, i*MAP_SIZE + MAP_SPACE, MAP_SIZE, MAP_SIZE, mem2dc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY); // 벽 이미지를 그린다
	//		}
	//	}
	//}
	//SelectObject(mem2dc, oldBit); // 벽 이미지로 셋팅된 oldBit를 해제한다.
	
	HPEN hPen, oldPen;
	hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	oldPen = (HPEN)SelectObject(mem1dc, hPen);
	for (int i = 0; i < MAP_CNUM; ++i) {
		for (int j = 0; j < MAP_CNUM; ++j) {
			if (map[i][j] == 1) {
				Rectangle(mem1dc, j*MAP_SIZE, i*MAP_SIZE + MAP_SPACE, (j + 1)*MAP_SIZE, (i + 1)*MAP_SIZE + MAP_SPACE);
			}
		}
	}
	
}

// 시간 출력
void Print_Time(HDC mem1dc, int time)
{
	char str[20];

	wsprintf(str, "Play Time : %d sec", time/1000);
	TextOut(mem1dc, 200, 20, str, strlen(str));
}

// 리셋 카운트 출력
void Print_Reset_Count(HDC mem1dc, int cnt)
{
	char str[30];

	wsprintf(str, "남은 변경 횟수 : %d번", END_RESET_COUNT - cnt);
	TextOut(mem1dc, 200, 40, str, strlen(str));
}

// 종료 메시지 출력
void Print_End_Message(HDC mem1dc, Player *p1, Player *p2)
{
	char str[30];
	int win_pnum;

	pdata_t pdata1, pdata2;

	pdata1 = p1->Get_Player_Info();
	pdata2 = p2->Get_Player_Info();

	// p1과 p2의 스코어를 비교하여 승자를 출력
	if (pdata1.score > pdata2.score)
		win_pnum = pdata1.pnum;
	else
		win_pnum = pdata2.pnum;

	wsprintf(str, "Player%d Victory!!", win_pnum);
	TextOut(mem1dc, 250, 400, str, strlen(str));
}

// 백그라운드 출력
void DrawBackground(HDC mem1dc, HDC mem2dc, RECT winRect, HBITMAP backBit)
{
	BITMAP bmp;
	HBITMAP oldBit;

	oldBit = (HBITMAP)SelectObject(mem2dc, backBit); // 백그라운드 이미지를 mem2dc에 셋팅한다

	GetObject(backBit, sizeof(BITMAP), &bmp); // 크기
	
	StretchBlt(mem1dc, 0, 0, winRect.right, winRect.bottom, mem2dc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY); // 백그라운드 이미지를 그린다
	SelectObject(mem2dc, oldBit); // 백그라운드 이미지로 셋팅된 oldBit를 해제한다.
}