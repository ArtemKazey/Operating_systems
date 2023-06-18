#include <condition_variable>
#include <iostream>
#include <thread>

void Work(long* arr, int n);
void MultElement(long* arr, int n, long* ptrProduct);

// 1. »нициализировать необходимые событи€ и критические секции.

std::condition_variable cv1;
std::mutex m1;

std::condition_variable cv2;
std::mutex m2;

std::mutex m_cs;

int main() {
  // 2.создать массив(тип long), размерность и элементы которого ввод€тс€
  // пользователем с консоли
  int n;
  std::cout << "n: ";
  std::cin >> n;
  long* arr = new long[n];
  std::cout << "elements of array: \n";
  for (int i = 0; i < n; i++) {
    std::cin >> arr[i];
  }

  // 3.вывести размерность и элементы исходного массива на консоль;
  std::cout << "Main: array: ";
  for (int i = 0; i < n; i++) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  // 4. запустить поток work
  std::thread work(Work, arr, n);

  // 5. запустить поток MultElement
  m_cs.lock();
  long product;
  std::thread multElement(MultElement, arr, n, &product);

  // 6. ƒождатьс€ сигнала потока work(использовать событие);
  std::unique_lock<std::mutex> lck1(m1);
  cv1.wait(lck1);

  // 7. ¬ыводить на экран элементы массива (итогового);
  std::cout << "Main: array: ";
  for (int i = 0; i < n; i++) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  // 8. известить поток MultElement о начале умножени€
  //cv_cs.notify_one();
  m_cs.unlock();

  // 9. ƒождатьс€ сигнала потока MultElement(использовать событие);
  std::unique_lock<std::mutex> lck2(m2);
  cv2.wait(lck2);
  // 10. вывести на экран результат работы потока MultElement;
  std::cout << "Main after multElement: product = " << product << std::endl;

  work.join();
  multElement.join();

  return 0;
}