#include <stdio.h>
#include <windows.h>
#include <iostream>

int main() {
  //������������� �������� �������������
  HANDLE hAEvent;
  char lpAEventName[] = "AEventName";

  HANDLE hBEvent;
  char lpBEventName[] = "BEventName";

  HANDLE hStopEvent;
  char lpStopEventName[] = "StopEventName";

  HANDLE hCEvent;
  char lpCEventName[] = "CEventName";

  HANDLE hDEvent;
  char lpDEventName[] = "DEventName";

  //���������� ���������� �������� ���������
  int activeCount = 5;
  //�������������� �������
  char lpSemaphore[] = "SemaphoreName";
  HANDLE hSemaphore = CreateSemaphore(NULL, activeCount, activeCount, lpSemaphore);
  if (hSemaphore == NULL) {
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "Semaphore is not created.\n";
    return -1;
  }

  HANDLE hHandles[3];
  // ������� �������, ���������� ���� ������� (A, B, S)
  hHandles[0] = CreateEvent(NULL, FALSE, FALSE, lpAEventName);
  if (hHandles[0] == NULL) {
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "Event A is not created.\n";
    return -1;
  }
  std::cout << "Event A created\n";

  hHandles[1] = CreateEvent(NULL, FALSE, FALSE, lpBEventName);
  if (hHandles[1] == NULL) {
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "Event B is not created.\n";
    return -1;
  }
  std::cout << "Event B created\n";

  hHandles[2] = CreateEvent(NULL, FALSE, FALSE, lpStopEventName);
  if (hHandles[2] == NULL) {
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "Event Stop is not created.\n";
    return -1;
  }
  std::cout << "Event Stop created\n";

   
  // ������� �������, ���������� ���� ������� (C, D)
  hCEvent = CreateEvent(NULL, FALSE, FALSE, lpCEventName);
   if (hCEvent == NULL) {
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "Event C is not created.\n";
    return -1;
  }
  std::cout << "Event C created\n";

  hDEvent = CreateEvent(NULL, FALSE, FALSE, lpDEventName);
  if (hDEvent == NULL) {
    std::cout << "GetLastError = " << GetLastError() << "\n";
    std::cout << "Event D is not created.\n";
    return -1;
  }
  std::cout << "Event D created\n";

  char lpParentApp[] = "parent.exe";
  char lpChildApp[] = "child.exe";

  //���� ���������� ��������� parent
  int n_parent;
  std::cout << "Number of parent processes: ";
  std::cin >> n_parent;

  //���� ���������� ��������� child
  int n_child;
  std::cout << "Number of child processes <= 4: ";
  std::cin >> n_child;
  if (n_child > 4) {
    n_child = 4;
  }

  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.cb = sizeof(STARTUPINFO);
  ZeroMemory(&pi, sizeof(pi));

  char lpParent[1000] = "";
  sprintf_s(lpParent, 1000, "%s %d", lpParentApp, n_parent);

  //������� �������� ���������� ��������� parent
  for (int i = 0; i < n_parent; i++) {
    if (!CreateProcess(NULL, lpParent, NULL, NULL, FALSE, CREATE_NEW_CONSOLE,
                       NULL, NULL, &si, &pi)) {
      std::cout << "GetLastError = " << GetLastError() << "\n";
      std::cout << "The parent process is not created.\n";
      std::cout << "Press any key to finish.\n";
      std::cin.get();
      return -1;
    }
    std::cout << "Create process Parent\n";
  }

  char lpChild[1000] = "";
  sprintf_s(lpChild, 1000, "%s", lpChildApp);

  //������� �������� ���������� ��������� child
  for (int i = 0; i < n_child; i++) {
    if (!CreateProcess(NULL, lpChild, NULL, NULL, FALSE, CREATE_NEW_CONSOLE,
                       NULL, NULL, &si, &pi)) {
      std::cout << "GetLastError = " << GetLastError() << "\n";
      std::cout << "The child process is not created.\n";
      std::cout << "Press any key to finish.\n";
      std::cin.get();
      return -1;
    }
    std::cout << "Create process child\n";
  }

  //���� ���������� ���������
  int n_messages;
  std::cout << "Number of messages: ";
  std::cin >> n_messages;

  for (int i = 0; i < n_messages; i++) {
    //�����������: ������� ��������� C ��� D �������� Child 
    //��� ��� ��������� A, B, S
    std::cout << "Enter command(C - message C, D - message D, W - wait): ";
    char command;
    std::cin >> command;
    std::cin.get();

    if (command == 'W') {
      //���, ���� ������� ������ ������ ������ �� �������� �������

      //���������� �������� � �������, ��� �������, ������� ����� ������
      DWORD dwWaitResult = WaitForMultipleObjects(3, hHandles, FALSE, INFINITE);

      //������������ �������� ���������, ����� ������� ���������������

      switch (dwWaitResult) {
        //hHandles[0] - ������� A - �����������������
        case WAIT_OBJECT_0 + 0:
          std::cout << "A event was signaled.\n";
          break;

        //hHandles[1] - ������� � - �����������������
        case WAIT_OBJECT_0 + 1:
          std::cout << "B event was signaled.\n";
          break;

        //hHandles[2] - ������� Stop - �����������������
        case WAIT_OBJECT_0 + 2:
          std::cout << "Stop event was signaled.\n";
          break;

        //������������ �������� �����������
        default:
          std::cout << "Wait error: " << GetLastError() << "\n";
      }
    } else if (command == 'C') {
      SetEvent(hCEvent);
      std::cout << "Message C is sent\n";
    } else {
      SetEvent(hDEvent);
      std::cout << "Message D is sent\n";
    }
  }

  CloseHandle(hCEvent);
  CloseHandle(hDEvent);
  CloseHandle(hHandles);

  std::cout << "Press any key to finish.\n";
  std::cin.get();
  return 0;
}