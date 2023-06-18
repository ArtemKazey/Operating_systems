#include <iostream>
#include <thread>
#include <condition_variable>

extern std::condition_variable cv1;
extern std::mutex m1; 

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

void Work(long* arr, int n) {
  std::cout << "work started\n";

  //1. ��������� � ������������ ��������� ��������,
  //��������� ��� ������ ����� ���������� ������ �������� � �������
  int delay;
  std::cout << "delay time (ms) : ";
  std::cin >> delay;

  std::cout << "Work: n = " << n << std::endl;

  //2. ����� � ������� ��������� ������� �����
  int i_write = 0;
  for (int i_read = 0; i_read < n; i_read++) {
    if (isValid(arr[i_read])) {
      arr[i_write] = arr[i_read];
      i_write++;
    }

    //3. ����� ������� �������� �������� �������� � ������� ��������� ��������� �������
    std::this_thread::sleep_for(std::chrono::milliseconds(delay)); 
  }
  for (; i_write < n; i_write++) {
    arr[i_write] = 0;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay)); 
  }

  //4. �������� �� ����� ����������� �������� ������� (���������)
  std::cout << "Work: ";
  for (int i = 0; i < n; i++) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  //5. ��������� ����� main � ������ ������
  std::unique_lock<std::mutex> lck(m1);
  cv1.notify_one(); 
}