#define XKOS_V5$MODULE    1
#define XKOS_V5$PROGRAM    1
#define XKOS_V5$INCLUDED_BY_KERNEL_SYSTEM_FILE    1
#define XKOS_V5$EXTERNLIB    1
#define XKOS_V5$STANDARD_LIBRARY    1
#define XKOS_V5$EXTERN_C_FUNCTION$streq    1
#define XKOS_V5$EXTERN_C_FUNCTION$clear    1
#define XKOS_V5$EXTERN_C_FUNCTION$scpy    1
#define XKOS_V5$EXTERN_C_FUNCTION$kgreenln    1
#define XKOS_V5$EXTERN_C_FUNCTION$read_word    1


char* write_buffer = "------------------------------";
char* last = "------------------------------";

void write() {
    for(;;) {
        read_word(write_buffer, 256, 0, 0);
        if(streq(write_buffer, "end") == 0)
            break;
        else if(streq(write_buffer, "clear") == 0)
            clear();
        else if(streq(write_buffer, "buffer") == 0)
            kpanicln(last, 1);
        else if(streq(write_buffer, "help") == 0){
            kgreenln("Use \"end\" to end write.");
            kgreenln("Use \"buffer\" to display the buffer.");
            kgreenln("Use \"clear\" to clear the TTY.");
        }
        scpy(last, write_buffer);
    }
}
