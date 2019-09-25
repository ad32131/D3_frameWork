#pragma once

#include<Windows.h>
#include "render.h"

class C_MYWIN
{
private:
	static C_MYWIN	* m_pMyWin;
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;

	C_RENDER	m_cRender;

private:
	C_MYWIN();

public:
	static void createMyWin();
	static C_MYWIN * getMyWin();
	static void releaseMyWin();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK myProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void init(HINSTANCE hInstance);
	void updateMsg();

};