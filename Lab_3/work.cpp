#include <Windows.h>
#include <iostream>

extern HANDLE hWorkEvent;

struct SParam {
  long* arr;
  int n;
  long product;
};

bool isValid(long x) {
  int d = 2;
  for (; d < x / 2; d++) {
    if (x % d == 0) {
      break;
    }
  }
  if (x == d * d) {
    return true;
  }
  return false;
}

DWORD WINAPI Work(LPVOID lpParameter) {
  std::cout << "work started\n";
  int delay;
  std::cout << "delay time: ";
  std::cin >> delay;

  SParam* pParam = (SParam*)lpParameter;

  std::cout << "Work: n = " << pParam->n << std::endl;

  int i_write = 0;
  for (int i_read = 0; i_read < pParam->n; i_read++) {
    if (isValid(pParam->arr[i_read])) {
      pParam->arr[i_write] = pParam->arr[i_read];
      i_write++;
    }
    Sleep(delay);
  }
  for (; i_write < pParam->n; i_write++) {
    pParam->arr[i_write] = 0;
    Sleep(delay);
  }

  std::cout << std::endl;
  SetEvent(hWorkEvent);
  
  return 0;
}