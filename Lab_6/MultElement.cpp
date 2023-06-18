#include <iostream>
#include <thread>
#include <mutex>

extern std::condition_variable cv_cs;
extern std::mutex m_cs;
extern std::condition_variable cv2;
extern std::mutex m2;

void MultElement(long* arr, int n, long* ptrProduct) {
  std::cout << "MultElement started\n";

  // 1. ��� �� ������ main ������ � ������ ������;
  m_cs.lock();
  m_cs.unlock();

  // 2. ��������� ������������ ��������� �� ��������� [0, 10] �� ���������
  // �������

  long product = 1;
  for (int i = 0; i < n; i++) {
    long elem = arr[i];
    if (elem > 0 && elem <= 10) {
      product *= elem;
    }
  }

  *ptrProduct = product;
  std::cout << "MultElement product = " << product << std::endl;

  // 3. ��������� ����� main � ������ ���������� (������������ �������);
  std::unique_lock<std::mutex> lck(m2);
  cv2.notify_one();
}