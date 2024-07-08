#ifndef XK_EXE_HXX
#define XK_EXE_HXX

/*
 * C code of a hello world program
 *
 * #include <Nitrix/KCALL.h> // CURSOR POSITION ALWAYS AT *(int*)(0x10000)
 *
 * void begin(void) {
 *   K_WRITE("Hello, world!");
 *   return;
 * }
*/

/*
 * That in assembly
 *
 * ; Stuff of Nitrix/KCALL.h
 *
 * .LC0:
 *   .string "Hello, world!"
 * begin:
 *   push rbp
 *   mov  rbp, rsp
 *   mov  edi, OFFSET FLAT:.LC0
 *   call K_WRITE
 *   nop
 *   pop  rbp
 *   ret
 *
*/

typedef struct {
  char magic[5];
  int  length;
  char *file;
} xk_exe;

#endif // XK_EXE_HXX
