#include <windows.h> // ������ ��� ����
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

void ReadMapData();																// �ؽ�Ʈ������ �о ���� �ε�(�� 0  �� 1)
void DrawMap(HDC mem1dc, HDC mem2dc, HBITMAP wallBit);							// �� ���
void DrawBackground(HDC mem1dc, HDC mem2dc, RECT winRect, HBITMAP backBit);		// ��׶��� ���
void Print_Time(HDC mem1dc, int time);											// ��� �ð� ���
void Print_Reset_Count(HDC mem1dc, int cnt);									// ���� ī��Ʈ ���
void Print_End_Message(HDC mem1dc, Player *p1, Player *p2);						// ���� �޼��� ���


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
	static bool game_state = TRUE;				// TRUE : ������, FALSE : ��������
	static int game_time = GAME_SPEED;			// ������Ʈ Ÿ�̸� �ӵ� (ms)
	static int reset_cnt = 0;					// ���� �� Ƚ��
	static unsigned long start_time, end_time;	// ������ ����� �ð�
	static RECT winRect;						// �����Ϸ��� ������ �ڵ�� ũ��
	static HBITMAP hBit1, oldBit1, oldBit2;		// ���� ���۸� ��Ʈ��
	static HBITMAP backBit;						// ��׶��� ��Ʈ��
	static HBITMAP wallBit;						// �� ��Ʈ��
	static Player *p1, *p2;						// �÷��̾� 1, 2
	static Item* it1;							// ������1
	static Collision* Col;						// �浹üũ Ŭ����

	switch (iMessage) {
	case WM_CREATE:
		srand(time(NULL));
		ReadMapData();   // ���� �׸������� ������ �о��
		GetClientRect(hWnd, &winRect); // ������ �����ǥ, ������ ũ�� ������ winRect���� ����

		backBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1)); // ��׶��� �̹��� �ε�
		wallBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALL_BIT)); // �� �̹��� �ε�

		// p1�� p2 ����, �÷��̾� ��ȣ�� ���ڰ����� ����
		p1 = new Player(1);
		p2 = new Player(2);

		// �� �÷��̾��� 1���� �������� ������ ���Ͽ� �ʱ�ȭ�Ѵ�
		if (rand() % 2 == 0)
		{	
			// P1 ����, P2 �Ϲ�
			p1->Init(TRUE);
			p2->Init(FALSE);
		}
		else
		{
			// P1 �Ϲ�, P2 ����
			p1->Init(FALSE);
			p2->Init(TRUE);
		}

		// ������ ��ü ����
		it1 = new Item;
		it1->Init();

		// �浹üũ ��ü ����
		Col = new Collision;

		// ���۽ð� üũ
		start_time = GetTickCount();

		// Ÿ�̸� ����
		SetTimer(hWnd, 1, 50, NULL);		// ���� ���۸� Ÿ�̸�
		SetTimer(hWnd, 2, game_time, NULL); // 2�� Ÿ�̸ӿ� time��ŭ �ð� ������ ����, time �ð����� WM_TIMER �߻� 

		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		mem1dc = CreateCompatibleDC(hdc);
		// hBit1���� ���� Ÿ���� ��µ� ��Ʈ���� ����, mem1dc�� ����
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);

		GetObject(hBit1, sizeof(BITMAP), &bmp); // ũ��

		// mem1dc�� �ִ� ������ hdc�� �ѷ��ش�.
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
		// ����Ű�� P1 ������ȯ
		p1->Key_Event();

		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:		// ���� ���۸� Ÿ�̸�
			hdc = GetDC(hWnd);

			GetClientRect(hWnd, &winRect);

			if (hBit1 == NULL) // hBit1�� hdc�� ȣȯ�ǰ� ������ش�.
				hBit1 = CreateCompatibleBitmap(hdc, winRect.right, winRect.bottom);

			mem1dc = CreateCompatibleDC(hdc);
			mem2dc = CreateCompatibleDC(mem1dc);

			oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);

			// ��׶��� �̹��� ���
			DrawBackground(mem1dc, mem2dc, winRect, backBit);

			// �� ���
			DrawMap(mem1dc, mem2dc, wallBit);

			// �÷��̾� ���
			p1->Draw(mem1dc);
			p2->Draw(mem1dc);

			// ������ ���
			it1->Draw(mem1dc);

			// �ð� ���
			Print_Time(mem1dc, GetTickCount() - start_time);

			// ���� ī��Ʈ ���
			Print_Reset_Count(mem1dc, reset_cnt);

			// ������ ����Ǹ� ���� �޽��� ���
			if(game_state == FALSE)
				Print_End_Message(mem1dc, p1, p2);

			DeleteDC(mem2dc);
			SelectObject(mem1dc, oldBit1);
			DeleteDC(mem1dc);
			ReleaseDC(hWnd, hdc);

			break;


		case 2:		// ������Ʈ Ÿ�̸� (�浹üũ)
			GetClientRect(hWnd, &winRect); // ������ �����ǥ, ������ ũ�� ������ winRect���� ����

			if (END_RESET_COUNT - reset_cnt <= 0)
				game_state = FALSE;


			// ���� �������
			if (game_state)
			{
				// �÷��̾� ������Ʈ
				p1->Update();
				p2->Update();

				// �÷��̾�� ���� �浹üũ
				Col->Player_to_Wall_Collision(p1);
				Col->Player_to_Wall_Collision(p2);

				// �÷��̾�� ��Ż �浹üũ
				Col->Player_to_Potal_Collision(p1);
				Col->Player_to_Potal_Collision(p2);

				// �÷��̾�� �浹üũ
				if (Col->Player_to_Player_Collision(p1, p2))
				{
					start_time = GetTickCount();
					reset_cnt++;
				}

				// �÷��̾�� ������ �浹üũ
				Col->Player_to_Item_Collision(p1, it1);
				Col->Player_to_Item_Collision(p2, it1);

				// ��� �ð��� CHANGE_TIME�� ������ �÷��̾ ����
				if (GetTickCount() - start_time > CHANGE_TIME)
				{
					p1->TimeOver_Reset();			// �÷��̾���� Ÿ�ӿ��� �������� �ʱ�ȭ
					p2->TimeOver_Reset();
					start_time = GetTickCount();	// ���۽ð� �ʱ�ȭ
					reset_cnt++;					// ���� ī��Ʈ�� 1 ���Ѵ�
				}
			}

			/*
				// ������ ȭ����� �浹üũ
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
		KillTimer(hWnd, 1); // 1�� Ÿ�̸��� ���� ����
		KillTimer(hWnd, 2); // 2�� Ÿ�̸��� ���� ����
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

// �� ���
void DrawMap(HDC mem1dc, HDC mem2dc, HBITMAP wallBit) 
{

	//BITMAP bmp;
	//HBITMAP oldBit;

	//oldBit = (HBITMAP)SelectObject(mem2dc, wallBit); // �� �̹����� mem2dc�� �����Ѵ�

	//GetObject(wallBit, sizeof(BITMAP), &bmp); // ũ��
	//for (int i = 0; i < MAP_CNUM; ++i) {
	//	for (int j = 0; j < MAP_CNUM; ++j) {
	//		if (map[i][j] == 1) {
	//			StretchBlt(mem1dc, j*MAP_SIZE, i*MAP_SIZE + MAP_SPACE, MAP_SIZE, MAP_SIZE, mem2dc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY); // �� �̹����� �׸���
	//		}
	//	}
	//}
	//SelectObject(mem2dc, oldBit); // �� �̹����� ���õ� oldBit�� �����Ѵ�.
	
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

// �ð� ���
void Print_Time(HDC mem1dc, int time)
{
	char str[20];

	wsprintf(str, "Play Time : %d sec", time/1000);
	TextOut(mem1dc, 200, 20, str, strlen(str));
}

// ���� ī��Ʈ ���
void Print_Reset_Count(HDC mem1dc, int cnt)
{
	char str[30];

	wsprintf(str, "���� ���� Ƚ�� : %d��", END_RESET_COUNT - cnt);
	TextOut(mem1dc, 200, 40, str, strlen(str));
}

// ���� �޽��� ���
void Print_End_Message(HDC mem1dc, Player *p1, Player *p2)
{
	char str[30];
	int win_pnum;

	pdata_t pdata1, pdata2;

	pdata1 = p1->Get_Player_Info();
	pdata2 = p2->Get_Player_Info();

	// p1�� p2�� ���ھ ���Ͽ� ���ڸ� ���
	if (pdata1.score > pdata2.score)
		win_pnum = pdata1.pnum;
	else
		win_pnum = pdata2.pnum;

	wsprintf(str, "Player%d Victory!!", win_pnum);
	TextOut(mem1dc, 250, 400, str, strlen(str));
}

// ��׶��� ���
void DrawBackground(HDC mem1dc, HDC mem2dc, RECT winRect, HBITMAP backBit)
{
	BITMAP bmp;
	HBITMAP oldBit;

	oldBit = (HBITMAP)SelectObject(mem2dc, backBit); // ��׶��� �̹����� mem2dc�� �����Ѵ�

	GetObject(backBit, sizeof(BITMAP), &bmp); // ũ��
	
	StretchBlt(mem1dc, 0, 0, winRect.right, winRect.bottom, mem2dc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY); // ��׶��� �̹����� �׸���
	SelectObject(mem2dc, oldBit); // ��׶��� �̹����� ���õ� oldBit�� �����Ѵ�.
}