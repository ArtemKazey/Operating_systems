#include <conio.h>
#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  _cprintf("argc = %d\n", argc);
  for (int i = 0; i < argc; i++) {
    _cputs(argv[i]);
    _cputs("\n");
  }

  int n = atoi(argv[1]);
  char X = argv[3][0];
  _cprintf("x=%c\n", X);
  
  int i_write = n - 1;
  for (int i_read = n - 1; i_read >= 0; i_read--) {
    if (argv[2][i_read] != X) {
      argv[2][i_write] = argv[2][i_read];
      i_write--;
    }
  }
  for (; i_write >= 0; i_write--) {
    argv[2][i_write] = X;
  }
  _cprintf("array=%s\n", argv[2]);
  
  _cputs("hello from child\n");
  _cputs("Press any key to finish.\n");
  _getch();
}