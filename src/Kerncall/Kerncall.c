#include <Drivers/drivers.h>
//0...
void K_WRITE(char* INPUT){kprint(INPUT);}
void K_CLEAR(){clear();}
void K_CUPDT(){update_cursor();}
void K_CREST(){reset_cursor();}
void K_READW(char* BUFFER, const int LENGTH, int ADDED_LENGTH){read_word(BUFFER, LENGTH, ADDED_LENGTH);}
