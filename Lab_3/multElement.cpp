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

  // 1. ��� �� ������ main ������ � ������ ������;
  EnterCriticalSection(&cs);
  LeaveCriticalSection(&cs);
  
  //2. ��������� ������������ ��������� �� ��������� [0, 10] �� ��������� �������
  SParam* pParam = (SParam*)lpParameter;

  long product = 1;
  for (int i = 0; i < pParam->n; i++) {
    long elem = pParam->arr[i];
    if (elem > 0 && elem <= 10) {
      product *= elem;
    }
  }

  pParam->product = product;

  //3. ��������� ����� main � ������ ���������� (������������ �������);
  SetEvent(hMultEvent);

  return 0; 
}