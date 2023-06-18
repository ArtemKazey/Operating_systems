#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <conio.h>

int main() {
  char lpSmallApp[] = "small.exe";

  int size;
  std::cout << "Enter size of array: ";
  std::cin >> size;

  int* arr = new int[size];
  std::cout << "Enter elements of array: ";
  for (int i = 0; i < size; i++) {
    std::cin >> arr[i];
  }

  int N;
  std::cout << "Enter N: ";
  std::cin >> N;

  _cprintf("size = %d\n", size);
  for (int i = 0; i < size; i++) {
    _cprintf("\t%d", arr[i]);
  }
  _cprintf("\nN = %d\n", N);

  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  HANDLE hWritePipe1, hReadPipe1, hInheritWritePipe1;
  HANDLE hWritePipe2, hReadPipe2, hInheritReadPipe2;
  //pipe 1 - server read, small write
  //pipe 2 - server write, small read
  // создаем анонимный канал
  if (!CreatePipe(&hReadPipe1, &hWritePipe1, NULL, 0)) {
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "Create pipe failed.\n";
    std::cout << "Press any key to finish.\n";
    std::cin.get();
    return -1;
  }

  // делаем наследуемый дубликат дескриптора hWritePipe1
  if (!DuplicateHandle(GetCurrentProcess(), hWritePipe1, GetCurrentProcess(),
                       &hInheritWritePipe1, 0, TRUE, DUPLICATE_SAME_ACCESS)) {
    std::cout << "Duplicate handle failed.\n";
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "Press any key to finish.\n";
    std::cin.get();
    return -1;
  }

  CloseHandle(hWritePipe1);

  if (!CreatePipe(&hReadPipe2, &hWritePipe2, NULL, 0)) {
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "Create pipe failed.\n";
    std::cout << "Press any key to finish.\n";
    std::cin.get();
    return -1;
  }
  
  // делаем наследуемый дубликат дескриптора hReadPipe2
  if (!DuplicateHandle(GetCurrentProcess(), hReadPipe2, GetCurrentProcess(),
                       &hInheritReadPipe2, 0, TRUE, DUPLICATE_SAME_ACCESS)) {
    std::cout << "Duplicate handle failed.\n";
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "Press any key to finish.\n";
    std::cin.get();
    return -1;
  }

  CloseHandle(hReadPipe2);

  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.cb = sizeof(STARTUPINFO);
  // ZeroMemory(&pi, sizeof(pi));

  // формируем командную строку
  char lpSmall[1000] = "";
  wsprintf(lpSmall, "small.exe %d %d", (int)hInheritWritePipe1, 
      (int)hInheritReadPipe2);
  //sprintf_s(lpSmall, 1000, "%s %d %s %d", lpSmallApp, size, arr, N);
  
  //создаем процесс
  if (!CreateProcess(NULL, lpSmall, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL,
                     NULL, &si, &pi)) {
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "The Small process is not created.\n";
    std::cout << "Press any key to finish.\n";
    std::cin.get();
    return -1;
  }

  // закрываем дескрипторы нового процесса
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  // закрываем ненужный дескриптор канала
  CloseHandle(hInheritWritePipe1);
  CloseHandle(hInheritReadPipe2);

  // пишем в анонимный канал
  //Передаёт размер массива, элементы массива и число N процессу Small.

  // pipe 1 - server read, small write
  // pipe 2 - server write, small read

  DWORD dwBytesWritten;
  WriteFile(hWritePipe2, &size, sizeof(size), &dwBytesWritten, NULL);
  _cprintf("size %d is written to the pipe.\n", size);
  for (int i = 0; i < size; i++) {
    
    if (!WriteFile(hWritePipe2, &arr[i], sizeof(arr[i]), &dwBytesWritten, NULL)) {
      std::cout << "Write to file failed.\n";
      std::cout << "GetLastError = " << GetLastError() << "\n";
      std::cout << "Press any key to finish.\n";
      std::cin.get();
      return -1;
    }

    _cprintf("Element %d is written to the pipe.\n", arr[i]);
  }
  WriteFile(hWritePipe2, &N, sizeof(N), &dwBytesWritten, NULL);
  _cprintf("N %d is written to the pipe.\n", N);

  // читаем из анонимного канала
  DWORD dwBytesRead;
  ReadFile(hReadPipe1, &size, sizeof(size), &dwBytesRead, NULL);
  _cprintf("\nSize %d is read from the pipe.\n", size);
  for (int i = 0; i < size; i++) {
    if (!ReadFile(hReadPipe1, &arr[i], sizeof(arr[i]), &dwBytesRead, NULL)) {
      std::cout << "Read from the pipe failed.\n";
      std::cout << "GetLastError = " << GetLastError() << "\n";
      std::cout << "Press any key to finish.\n";
      std::cin.get();
      return -1;
    }
    _cprintf("Element %d is read from the pipe.\n", arr[i]);
  }

  // закрываем дескриптор канала
  CloseHandle(hReadPipe1);
  CloseHandle(hWritePipe2);

  _cprintf("result = ");
  for (int i = 0; i < size; i++) {
    _cprintf("\t%d", arr[i]);
  }
  _cprintf("\n");

  std::cout << "\nThe process finished.\n";
  std::cout << "Press any key to exit.\n";
  std::cin.get();

  return 0;
}