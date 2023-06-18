#include <condition_variable>
#include <iostream>
#include <thread>

void Work(long* arr, int n);
void MultElement(long* arr, int n, long* ptrProduct);

// 1. ���������������� ����������� ������� � ����������� ������.

std::condition_variable cv1;
std::mutex m1;

std::condition_variable cv2;
std::mutex m2;

std::mutex m_cs;

int main() {
  // 2.������� ������(��� long), ����������� � �������� �������� ��������
  // ������������� � �������
  int n;
  std::cout << "n: ";
  std::cin >> n;
  long* arr = new long[n];
  std::cout << "elements of array: \n";
  for (int i = 0; i < n; i++) {
    std::cin >> arr[i];
  }

  // 3.������� ����������� � �������� ��������� ������� �� �������;
  std::cout << "Main: array: ";
  for (int i = 0; i < n; i++) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  // 4. ��������� ����� work
  std::thread work(Work, arr, n);

  // 5. ��������� ����� MultElement
  m_cs.lock();
  long product;
  std::thread multElement(MultElement, arr, n, &product);

  // 6. ��������� ������� ������ work(������������ �������);
  std::unique_lock<std::mutex> lck1(m1);
  cv1.wait(lck1);

  // 7. �������� �� ����� �������� ������� (���������);
  std::cout << "Main: array: ";
  for (int i = 0; i < n; i++) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  // 8. ��������� ����� MultElement � ������ ���������
  //cv_cs.notify_one();
  m_cs.unlock();

  // 9. ��������� ������� ������ MultElement(������������ �������);
  std::unique_lock<std::mutex> lck2(m2);
  cv2.wait(lck2);
  // 10. ������� �� ����� ��������� ������ ������ MultElement;
  std::cout << "Main after multElement: product = " << product << std::endl;

  work.join();
  multElement.join();

  return 0;
}