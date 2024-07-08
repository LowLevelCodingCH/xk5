#include <Executable/xk_exe.h>

/*
 *
 * xk_exe exe;
 * exe.magic     = "XKE.5";
 * exe.end       = "\0\0\0\0\55\66\77\88\01\02\03\04";
 * exe.data.data = "Hi\0";
 * exe.code.data = "\b8\00\bf\05\cd\80";
 *
*/

//typedef struct {
//  int   from;
//  int   to;
//  char* what;
//} from_to;
//
//xk_exe LoadEXE(char* executable){
//  xk_exe exe;
//
//  from_to magic = {
//    .what = executable,
//    .from = 0,
//    .to   = 4
//  };
//
//  from_to data = {
//    .what = executable,
//    .from = 5,
//    .to   = 10240
//  };
//
//  from_to code = {
//    .what = executable,
//    .from = 1025,
//    .to   = 10240
//  };
//
//  from_to end = {
//    .what = executable,
//    .from = 10241,
//    .to   = 10253
//  };
//
//  return exe;
//}
//
