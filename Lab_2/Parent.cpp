#include <conio.h>
#include <windows.h>
#include <stdio.h>

int main() {
  char lpszAppName[] = "Child.exe";

  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.cb = sizeof(STARTUPINFO);
  si.dwFillAttribute = FOREGROUND_GREEN;
  si.dwFlags = STARTF_USEFILLATTRIBUTE;
  ZeroMemory(&pi, sizeof(pi));

  int n;
  char buffer[10];
  _cputs("size: \n");
  size_t pSizeRead;
  _cgets_s(buffer, 10, &pSizeRead);

  n = atoi(buffer);

  _cputs("\nelements of array: \n");
  char* arr = new char[n + 1];
  for (int i = 0; i < n; i++) {
    arr[i] = _getche();
  }
  arr[n] = 0;

  _cputs("\nX: \n");
  char X = _getche();

  char lpszCommandLine[1000] = "";
  sprintf_s(lpszCommandLine, 1000, "%s %d %s %c", lpszAppName, n, arr, X);
  
  _cprintf("\nlpszCommandLine = %s\n", lpszCommandLine);

  if (!CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE,
                     NULL, NULL, &si, &pi)) {
    _cprintf("GetLastError = %d\n", GetLastError());
    _cputs("The new process is not created.\n");
    _cputs("Press any key to finish.\n");
    _getch();
    return -1;
  } else {
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return 0;
  }
}