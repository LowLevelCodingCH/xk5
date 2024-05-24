#include <Drivers/drivers.h>

static char* tty = "";

void ttywrite(char* info) {
  scpy(tty, info);
}
