// frameWork.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	C_MYWIN::createMyWin();
	C_MYWIN::getMyWin()->init(hInstance);
	C_MYWIN::getMyWin()->updateMsg();
	C_MYWIN::releaseMyWin();


	return 0;
}

