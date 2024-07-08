#include <Drivers/drivers.h>
//0...

#define NXAPI

typedef char* XKSTR;

void NXAPI K_PUTLN(int HOW_MUCH){kputln(HOW_MUCH);}
void NXAPI K_WRITE(XKSTR INPUT){kprint(INPUT);}
void NXAPI K_CLEAR(){clear();}
void NXAPI K_CUPDT(){update_cursor();}
void NXAPI K_CREST(){reset_cursor();}
void NXAPI K_READW(XKSTR BUFFER, const int LENGTH, int ADDED_LENGTH, int BACKSPACE_LIMIT){read_word(BUFFER, LENGTH, ADDED_LENGTH, BACKSPACE_LIMIT);}
void NXAPI K_SSIZE(XKSTR BUFFER, int* LENGTH){*LENGTH = ssize(BUFFER);}
void NXAPI K_BEEPT(uint32_t PITCH, uint32_t TIME){beep(PITCH, TIME);}
void NXAPI K_SPKIN(){init_speaker();}
void NXAPI K_SPOFF(){speaker_off();}
void NXAPI K_PTONE(uint32_t FREQUENCY){play_tone(FREQUENCY);}
void NXAPI K_PITIN(){init_pit();}
void NXAPI K_WAIMS(unsigned int MILLISECONDS){wait_ms(MILLISECONDS);}
void NXAPI K_HALTK(unsigned char PERMISSIONS){if(PERMISSIONS == 2 /*Kernel: 2, Driver: 1, User: 0*/)halt();}
