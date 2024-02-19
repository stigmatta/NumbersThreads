#include "MultithreadDlg.h"

CMultithreadDlg* CMultithreadDlg::ptr = NULL;

CMultithreadDlg::CMultithreadDlg(void)
{
	ptr = this;
}

CMultithreadDlg::~CMultithreadDlg(void)
{

}

void CMultithreadDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

DWORD WINAPI Thread1(LPVOID lp) {
	HWND hList = (HWND)lp;
	TCHAR buffer[256]; 
	for (int i = 2; i < 100000; i++) {
		bool prime = true;
		for (int j = 2; j<= i/2; j++) {
			if (i % j == 0) {
				prime = false;
				break;
			}
		}
		if (prime) {
			wsprintf(buffer, _T("%d"), i);
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)buffer);
		}
		Sleep(50); 
	}
	return 0;
}

DWORD WINAPI Thread2(LPVOID lp) {
	HWND hList = (HWND)lp;
	TCHAR buffer[256];
	for (int i = 2; i < 100000; i++) {
		if (i % 2 == 0) {
			wsprintf(buffer, _T("%d"), i);
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)buffer);
		}
			
		Sleep(50);
	}
	return 0;
}
DWORD WINAPI Thread3(LPVOID lp) {
	HWND hList = (HWND)lp;
	TCHAR buffer[256];
	for (int i = 1; i < 20000; i++) {
		wsprintf(buffer, _T("%d"), i);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)buffer);
		Sleep(50);
	}
	return 0;
}
BOOL CMultithreadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	srand(time(0));
	hList1 = GetDlgItem(hwnd, IDC_LIST1);
	hList2 = GetDlgItem(hwnd, IDC_LIST2);
	hList3 = GetDlgItem(hwnd, IDC_LIST3);
	hPlay1 = GetDlgItem(hwnd, IDC_BUTTON1);
	hPlay2 = GetDlgItem(hwnd, IDC_BUTTON3);
	hPlay3 = GetDlgItem(hwnd, IDC_BUTTON5);

	Th1 = CreateThread(NULL, 0, Thread1, hList1, CREATE_SUSPENDED, NULL);
	Th2 = CreateThread(NULL, 0, Thread2, hList2, CREATE_SUSPENDED, NULL);
	Th3 = CreateThread(NULL, 0, Thread3, hList3, CREATE_SUSPENDED, NULL);
	return TRUE;
}

void CMultithreadDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_BUTTON1)
	{
		static BOOL flag = FALSE;
		if (flag)
		{
			SuspendThread(Th1);
			SetWindowText(hPlay1, TEXT("Пуск"));
		}
		else
		{
			ResumeThread(Th1);
			SetWindowText(hPlay1, TEXT("Пауза"));
		}
		flag = !flag;
	}
	else if (id == IDC_BUTTON3)
	{
		static BOOL flag = FALSE;
		if (flag)
		{
			SuspendThread(Th2);
			SetWindowText(hPlay2, TEXT("Пуск"));
		}
		else
		{
			ResumeThread(Th2);
			SetWindowText(hPlay2, TEXT("Пауза"));
		}
		flag = !flag;
	}
	else if (id == IDC_BUTTON5)
	{
		static BOOL flag = FALSE;
		if (flag)
		{
			SuspendThread(Th3);
			SetWindowText(hPlay3, TEXT("Пуск"));
		}
		else
		{
			ResumeThread(Th3);
			SetWindowText(hPlay3, TEXT("Пауза"));
		}
		flag = !flag;
	}
	else if (id == IDC_BUTTON2)
	{
		TerminateThread(Th1, 0);
		CloseHandle(Th1);
		EnableWindow(hPlay1, FALSE);
		SendMessage(hList1, LB_RESETCONTENT, 0, 0);
	}
	else if (id == IDC_BUTTON4)
	{
		TerminateThread(Th2, 0);
		CloseHandle(Th2);
		EnableWindow(hPlay2, FALSE);
		SendMessage(hList2, LB_RESETCONTENT, 0, 0);
	}
	else if (id == IDC_BUTTON6)
	{
		TerminateThread(Th3, 0);
		CloseHandle(Th3);
		EnableWindow(hPlay3, FALSE);
		SendMessage(hList3, LB_RESETCONTENT, 0, 0);
	}
	else if (id == IDCANCEL||id==IDOK) {
		EndDialog(hwnd, 0);
	}
}

BOOL CALLBACK CMultithreadDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}