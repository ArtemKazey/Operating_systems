#include <windows.h>
#include <stdio.h>
#include <iostream>

HANDLE hSemaphore;
HANDLE hAEvent;
char lpAEventName[] = "AEventName";
HANDLE hBEvent;
char lpBEventName[] = "BEventName";
HANDLE hStopEvent;
char lpStopEventName[] = "StopEventName";

int main(int argc, char* argv[]) {
  std::cout << "I am parent" << std::endl;

   //семафор
  char lpSemaphore[] = "SemaphoreName";
  HANDLE hSemaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, lpSemaphore);
  if (hSemaphore == NULL) {
    std::cout << "Open semaphore failed. GetLastError = " << GetLastError() << "\n";
    std::cout << "Press any key to exit.\n";
    std::cin.get();
    return -1;
  }

 hAEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpAEventName);
  if (hAEvent == NULL) {
    std::cout << "Open event A failed. GetLastError = " << GetLastError()
              << std::endl;
    std::cout << "Press any key to exit." << std::endl;
    std::cin.get();

    return -1;
  }
  std::cout << "Event A is opened\n";

  hBEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpBEventName);
  if (hBEvent == NULL) {
    std::cout << "Open event B failed. GetLastError = " << GetLastError()
              << std::endl;
    std::cout << "Press any key to exit." << std::endl;
    std::cin.get();

    return -1;
  }
  std::cout << "Event B is opened\n";

  hStopEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpStopEventName);
  if (hStopEvent == NULL) {
    std::cout << "Open event Stop failed. GetLastError = " << GetLastError()
              << std::endl;
    std::cout << "Press any key to exit." << std::endl;
    std::cin.get();

    return -1;
  }
  std::cout << "Event Stop is opened\n";

  //ждём, пока сможет стать активным
  WaitForSingleObject(hSemaphore, INFINITE);

  //запрашивает с консоли сообщения, 
  //состоящее «А» или «B» и передает их (по одному) процессу Boss
  while (true) {
    std::cout << "Enter command(A - message A, B - message B, S - Stop): ";
    char command;
    std::cin >> command;
    std::cin.get();
    switch (command) {
      case 'A':
        SetEvent(hAEvent);
        std::cout << "Message A is sent\n";
        break;
      case 'B':
        SetEvent(hBEvent);
        std::cout << "Message B is sent\n";
        break;
      case 'S':
        SetEvent(hStopEvent);

        //увеличиваем значение семафора на 1
        ReleaseSemaphore(hSemaphore, 1, NULL);

        CloseHandle(hSemaphore);
        CloseHandle(hAEvent);
        CloseHandle(hBEvent);
        CloseHandle(hStopEvent);

        std::cout << "Process is finished\n";
        std::cout << "Press any key to finish.\n";
        std::cin.get();
        return 0;
    }
  }
}