#include <Windows.h>
#include <iostream>

DWORD WINAPI Work(LPVOID lpParameters);
DWORD WINAPI MultElement(LPVOID lpParameters);

HANDLE hWorkEvent, hMultEvent;
CRITICAL_SECTION cs;

struct SParam {
  long* arr;
  int n;
  long product;
};

int main() {
  InitializeCriticalSection(&cs);

  hWorkEvent = CreateEvent(NULL, false, false, NULL);
  if (hWorkEvent == NULL) {
    std::cout << "CreateEvent Work error = " << GetLastError();
    return -1;
  }
  hMultEvent = CreateEvent(NULL, false, false, NULL);
  if (hMultEvent == NULL) {
    std::cout << "CreateEvent MultElement error = " << GetLastError();
    return -1;
  }

  int n;
  std::cout << "n: ";
  std::cin >> n;
  long* arr = new long[n];
  std::cout << "elements of array: \n";
  for (int i = 0; i < n; i++) {
    std::cin >>  arr[i];
  }

  std::cout << "array: ";
  for (int i = 0; i < n; i++) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  HANDLE hWork, hMult;
  DWORD idWork, idMult;

  SParam param1 = {arr, n};
  SParam param2 = {arr, n};

  hWork = CreateThread(NULL, 0, Work, &param1, 0, &idWork);
  if (hWork == NULL) {
    std::cout << "CreateThread Work error = " << GetLastError() << "\n";
    return -1;
  }

  EnterCriticalSection(&cs);

  hMult = CreateThread(NULL, 0, MultElement, &param2, 0, &idMult);
  if (hMult == NULL) {
    std::cout << "CreateThread MultElement error = " << GetLastError() << "\n";
    return -1;
  }

  //6. ��������� ������� ������ work(������������ �������); 
  WaitForSingleObject(hWorkEvent, INFINITE);

  //7. �������� �� ����� �������� �������(���������);
  std::cout << "array after Work: ";
  for (int i = 0; i < n; i++) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  //8. ��������� ����� MultElement � ������ ���������
  LeaveCriticalSection(&cs);
      
  //9. ��������� ������� ������ MultElement(������������ �������);
  WaitForSingleObject(hMultEvent, INFINITE);

  //10. ������� �� ����� ��������� ������ ������ MultElement;
  std::cout << "product after MultElement = " << param2.product << std::endl;

  // ���� ���� ����� Work �������� ������
  WaitForSingleObject(hWork, INFINITE);
  // ���� ���� ����� Mult �������� ������
  WaitForSingleObject(hMult, INFINITE);

  CloseHandle(hWork);
  CloseHandle(hMult);
  CloseHandle(hWorkEvent);
  CloseHandle(hMultEvent);
  DeleteCriticalSection(&cs);

  return 0;
}