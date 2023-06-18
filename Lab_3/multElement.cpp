#include <Windows.h>
#include <iostream>

extern HANDLE hMultEvent;
extern CRITICAL_SECTION cs;

struct SParam {
  long* arr;
  int n;
  long product;
};

DWORD WINAPI MultElement(LPVOID lpParameter) { 
  std::cout << "MultElement started\n";

  // 1. ждёт от потока main сигнал о начале работы;
  EnterCriticalSection(&cs);
  LeaveCriticalSection(&cs);
  
  //2. выполнить произведение элементов из диапазона [0, 10] из итогового массива
  SParam* pParam = (SParam*)lpParameter;

  long product = 1;
  for (int i = 0; i < pParam->n; i++) {
    long elem = pParam->arr[i];
    if (elem > 0 && elem <= 10) {
      product *= elem;
    }
  }

  pParam->product = product;

  //3. известить поток main о выводе результата (использовать событие);
  SetEvent(hMultEvent);

  return 0; 
}