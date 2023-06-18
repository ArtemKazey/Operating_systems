#include <conio.h>
#include <windows.h>
#include <iostream>

int main(int argc, char *argv[]) {
  HANDLE hWritePipe, hReadPipe;
  
  // преобразуем символьное представление дескриптора в число
  hWritePipe = (HANDLE)atoi(argv[1]);
  hReadPipe = (HANDLE)atoi(argv[2]);

  // ждем команды о начале работы
  std::cout << "Press any key to start communication.\n";
  std::cin.get();
  int size;
  int *arr;
  int N;

  // читаем из анонимного канала
  DWORD dwBytesRead;
  ReadFile(hReadPipe, &size, sizeof(size), &dwBytesRead, NULL);
  _cprintf("size %d is read from the pipe \n", size);
  arr = new int[size];
  for (int i = 0; i < size; i++) {   
    if (!ReadFile(hReadPipe, &arr[i], sizeof(arr[i]), &dwBytesRead, NULL)) {
      std::cout << "Read from the pipe failed.\n";
      std::cout << "GetLastError = " << GetLastError() << "\n";
      std::cout << "Press any key to finish.\n";
      std::cin.get();
      return -1;
    }
    _cprintf("Element %d is read from the pipe.\n", arr[i]);
  }
  ReadFile(hReadPipe, &N, sizeof(N), &dwBytesRead, NULL);
  _cprintf("N %d is read from the pipe \n", N);

  int i_write = 0;
  for (int i_read = 0; i_read < size; i_read++) {
    if (arr[i_read] > 0 && arr[i_read] < N) {
      arr[i_write] = arr[i_read];
      i_write++;
    }
  }
  size = i_write;

  // пишем в анонимный канал
  DWORD dwBytesWritten;
  WriteFile(hWritePipe, &size, sizeof(size), &dwBytesWritten, NULL);
  _cprintf("\nSize %d is written to the pipe.\n", size);
  for (int i = 0; i < size; i++) {
    
    if (!WriteFile(hWritePipe, &arr[i], sizeof(arr[i]), &dwBytesWritten, NULL)) {
      std::cout << "Write to file failed.\n";
      std::cout << "GetLastError = " << GetLastError() << "\n";
      std::cout << "Press any key to finish.\n";
      std::cin.get();
      return -1;
    }
    _cprintf("Element %d is written to the pipe.\n", arr[i]);
  }

  // закрываем дескриптор канала
  CloseHandle(hWritePipe);
  CloseHandle(hReadPipe);

  _cprintf("result = ");
  for (int i = 0; i < size; i++) {
    _cprintf("\t%d", arr[i]);
  }
  _cprintf("\n");

  std::cout << "\nThe process finished writing to the pipe.\n";
  std::cout << "Press any key to exit.\n";
  std::cin.get();

  return 0;
}
