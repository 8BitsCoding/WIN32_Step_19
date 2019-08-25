// Project1.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Project1.h"

#include <stdlib.h>
#include <time.h>

#define MAX_COUNT 100

struct Circle {
	int x;
	int y;
	int r;
	COLORREF color;
};

Circle g_pos[MAX_COUNT];
RECT g_rect;
HDC gh_mem_dc;
HBITMAP gh_mem_bmp;

void MakeCircleData(Circle * ap_data, RECT * ap_rect)
{
	ap_data->x = rand() % ap_rect->right;
	ap_data->y = rand() % ap_rect->bottom;
	ap_data->r = rand() % 50 + 10;
	ap_data->color = RGB(rand() % 256, rand() % 256, rand() % 256);
}

void DrawEllipse(HDC ah_dc)
{
	Circle *p = g_pos;
	HBRUSH h_brush;
	HGDIOBJ h_old_brush = GetCurrentObject(ah_dc, OBJ_BRUSH);
	HGDIOBJ h_old_pen = SelectObject(ah_dc, GetStockObject(NULL_PEN));

	FillRect(ah_dc, &g_rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

	for (int i = 0; i < MAX_COUNT; i++) {
		h_brush = CreateSolidBrush(p->color);
		h_old_brush = SelectObject(ah_dc, h_brush);
		Ellipse(ah_dc, p->x - p->r, p->y - p->r, p->x + p->r, p->y + p->r);
		p++;

		DeleteObject(h_brush);
	}

	SelectObject(ah_dc, h_old_pen);
	SelectObject(ah_dc, h_old_brush);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC h_dc = BeginPaint(hWnd, &ps);

		BitBlt(h_dc, 0, 0, g_rect.right, g_rect.bottom, gh_mem_dc, 0, 0, SRCCOPY);
	
		EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_CREATE) {
		srand((unsigned int)time(NULL));
		GetClientRect(hWnd, &g_rect);

		HDC h_dc = GetDC(hWnd);

		gh_mem_dc = CreateCompatibleDC(h_dc);
		gh_mem_bmp = CreateCompatibleBitmap(h_dc, g_rect.right, g_rect.bottom);
		SelectObject(gh_mem_dc, gh_mem_bmp);

		ReleaseDC(hWnd, h_dc);

		for (int i = 0; i < MAX_COUNT; i++) {
			MakeCircleData(g_pos + i, &g_rect);
			/*
			g_pos[i].x = rand() % r.right;
			g_pos[i].y = rand() % r.bottom;
			g_pos[i].r = rand() % 50 + 10;
			g_pos[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
			*/
		}

		SetTimer(hWnd, 1, 50, NULL);
		return 0;
	}
	else if (uMsg == WM_TIMER) {
		if (wParam == 1) {
			Circle *p = g_pos;
			for (int i = 0; i < MAX_COUNT; i++) {
				p->r--;
				if (p->r <= 1) {
					MakeCircleData(p, &g_rect);
				}
				p++;
			}
			DrawEllipse(gh_mem_dc);
			//InvalidateRect(hWnd, NULL, TRUE);
			InvalidateRect(hWnd, NULL, FALSE);
		}
	}
	else if (uMsg == WM_SIZE) {
		GetClientRect(hWnd, &g_rect);
		if (gh_mem_bmp != NULL) DeleteObject(gh_mem_bmp);

		HDC h_dc = GetDC(hWnd);

		gh_mem_dc = CreateCompatibleDC(h_dc);
		gh_mem_bmp = CreateCompatibleBitmap(h_dc, g_rect.right, g_rect.bottom);
		SelectObject(gh_mem_dc, gh_mem_bmp);
		FillRect(gh_mem_dc, &g_rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

		ReleaseDC(hWnd, h_dc);
	}
	else if (uMsg == WM_DESTROY) {
		KillTimer(hWnd, 1);
		DeleteObject(gh_mem_bmp);
		DeleteDC(gh_mem_dc);
		PostQuitMessage(0);
	}
		

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;

	wchar_t my_class_name[] = L"tipssoft";

	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = my_class_name;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);

	HWND hWnd = CreateWindow(my_class_name, L"www.tipssoft.com",
		WS_OVERLAPPEDWINDOW, 100, 90, 400, 350, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}