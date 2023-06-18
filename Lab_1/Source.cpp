#include <windows.h>
#include <iostream>
#include <vector>
#include <process.h>

using namespace std;

  unsigned _stdcall worker(void* array) {
  std::vector<int> temp = *(std::vector<int> *)array;
  int x;
  cout << "x: ";
  cin >> x;

  for (int i = 0; i < temp.size(); i++) {
    if (temp[i] < x) {
      cout << temp[i] << " ";
    }
    Sleep(100);
  }

  return 0;
}

int main() {
  int n;
  cout << "n: ";
  cin >> n;
  vector<int> array(n);
  cout << endl << "array: ";
 
  for (int i = 0; i < n; i++) {
    cin >> array[i];
  }

  HANDLE worker_desc;
  DWORD IDThread;

  int delay;
  cout << "delay time: ";
  cin >> delay;

  worker_desc = (HANDLE)_beginthreadex(NULL, 0, worker, &array, 0,(unsigned int *) &IDThread);

  if (worker_desc == NULL) {
    return GetLastError();
  }
  
  SuspendThread(worker_desc);
  Sleep(delay);
  ResumeThread(worker_desc);

  WaitForSingleObject(worker_desc, 2);

  CloseHandle(worker_desc);

  return 0;
}