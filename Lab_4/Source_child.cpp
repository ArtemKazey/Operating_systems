#include <windows.h>
#include <iostream>


int main() {
  HANDLE hSemaphore;
  char lpSemaphore[] = "SemaphoreName";

  //HANDLE hCEvent;
  char lpCEventName[] = "CEventName";

  //HANDLE hDEvent;
  char lpDEventName[] = "DEventName";

  HANDLE hStopEvent;
  char lpStopEventName[] = "StopEventName";

  std::cout << "I am child" << std::endl;

  HANDLE hHandles[2];
  hHandles[0] = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpCEventName);
  if (hHandles[0] == NULL) {
    std::cout << "Open event C failed. GetLastError = " << GetLastError() << "\n";
    std::cout << "Press any key to exit." << std::endl;
    std::cin.get();
    return -1;
  }

  hHandles[1] = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpDEventName);
  if (hHandles[1] == NULL) {
    std::cout << "Open event D failed. GetLastError = " << GetLastError() << "\n";
    std::cout << "Press any key to exit." << std::endl;
    std::cin.get();
    return -1;
  }

  hStopEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpStopEventName);
  if (hStopEvent == NULL) {
    std::cout << "Open event Stop failed. GetLastError = " << GetLastError()
              << "\n";
    std::cout << "Press any key to exit." << std::endl;
    std::cin.get();
    return -1;
  }
  hSemaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, lpSemaphore);
  if (hSemaphore == NULL) {
    std::cout << "Open semaphore failed. GetLastError = " << GetLastError()
              << "\n";
    std::cout << "Press any key to exit.\n";
    std::cin.get();
    return -1;
  }

  //ждём, пока сможет стать активным
  WaitForSingleObject(hSemaphore, INFINITE);

  //запрашивает с консоли команду
  while (true) {
    std::cout << "Enter command(W - Wait message C or D from Boss, S - Stop): ";
    char command;
    std::cin >> command;
    std::cin.get();

    if (command == 'W') {

      DWORD dwWaitResult = WaitForMultipleObjects(2, hHandles, FALSE, INFINITE);

      switch (dwWaitResult) {
        // hHandles[0] - событие C - сигнализировалось
        case WAIT_OBJECT_0 + 0:

          std::cout << "C event was signaled.\n";
          break;

        // hHandles[1] - событие D - сигнализировалось
        case WAIT_OBJECT_0 + 1:
          std::cout << "D event was signaled.\n";
          break;

        default:
          std::cout << "Wait error: " << GetLastError() << "\n";
      }
    } else {
      SetEvent(hStopEvent);
      //увеличиваем значение семафора на 1
      ReleaseSemaphore(hSemaphore, 1, NULL);

      CloseHandle(hSemaphore);
      CloseHandle(hStopEvent);
      CloseHandle(hHandles);

      std::cout << "Process is finished\n";
      std::cout << "Press any key to finish.\n";
      std::cin.get();
      return 0;
    }
  }
}