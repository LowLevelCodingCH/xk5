/*
     |----------------------------------------------------------------------------------------------------|
     |   |_ This is the only file which has "readable" code.                                              |
     |   |_ Follow the "guidelines" which the context of this gives you.                                  |
     |   |_ This is the main kernel file.                                                                 |
     |   |_ If you wanna make a commit, please follow this style _everywhere_ (even where it looks bad).  |
     |   |_ Please help me cleanup the code in the other files following.                                 |
     |   |_ Test AT LEAST 5 times before making a pull request. To test:                                  |
     |   |    |_ XKFS                                                                                     |
     |   |    |_ Printf                                                                                   |
     |   |    |_ Tasks                                                                                    |
     |   |    |_ PIT Sound                                                                                |
     |   |    |_ Users                                                                                    |
     |   |    |_ DiskIO                                                                                   |
     |   |    |_ CMOS Time                                                                                |
     |   |    |_ Keyboard                                                                                 |
     |   |_ When making NOTEs, please do it like this.                                                    |
     |   |_ Doesn't work on Windows since the crosscompilers and QEMU versions are not good.              |
     |   |_ I will try to make it work on Windows though                                                  |
     |----------------------------------------------------------------------------------------------------|
*/

/* I do not know why, but this is important, otherwise it will not work! */
int __stack_chk_fail_local = 0;

/* Maybe your compiler needs this, mine once errored this (not anymore btw), so I'll put it here too */
int __GLOBAL_OFFSET_TABLE_ = 0;
int __GLOBAL_OFFSET_TABLE  = 0;

/* Prints log then halts */
//#define __Kernel_Print_Log_Then_Halt__

#include <nitrix/std.h>
#include <nitrix/task.h>

#include <jitrix/mario.h>
#include <jitrix/textkb.h>
#include <jitrix/pit_sound.h>
#include <jitrix/diskio.h>
#include <jitrix/cmos.h>

#include <litrix/time.h>
#include <litrix/misc.h>
#include <litrix/xkfs.h>
#include <litrix/users.h>
#include <litrix/executable.h>
#include <litrix/syscalls.h>
#include <litrix/scheduler.h>

#include <nitrix/idt.h>
#include <nitrix/kerneltask.h>


#ifdef _WIN32
#error "This does NOT work on windows, support will be added soon!"
#endif

/* Like SystemD or OpenRC, it is an init system */

Task SystemX = {
    .running = TASK_RUNNING_FLAG,        // Runs    (If it runs or not)
    .length  = TASK_FOREVER_FLAG,        // Forever (-1: Forever. Everything else is the length of the task in ticks)
    .current_tick = 0,                   // Tick    (What SystemTick it is right now, to know when to exit)
    .name = "SystemX",                   // Name    (Name)
    .hasupdate = 1,                      // Unused  (Was meant to tell if it has an update function)
    .update = &Nx_KernelUpdate,          // Update  (Called every tick)     (cfiles/nitrix/kerneltask.h)
    .init = &Nx_KernelInit,              // Init    (Called once)           (cfiles/nitrix/kerneltask.h)
    .exists = TASK_EXIST_FLAG            // Exists  (TASK_EXIST_FLAG = 121. To prevent from NULL from being executed. If this isn't here, bugs are hard to find, it happened, trust me)
};

void Nx_Kernel(void)
{
    if(XK_IsComputerOn() != 1) {
        kprintln("misc: Computer is not turned on, please turn it on");
        asm("hlt");
    }
    if(XK_IsComputerOnFire() == 1) {
        kprintln("misc: Computer is on fire, please use sand, not water");
        asm("hlt");
    }

    klog_println("kernel: Initializing Scheduler & Multitasking");

    /*
          Appends the SystemX task to the tasklist (Task* _glob_tasks[16])
          If it becomes more complex, please add error handling.
    */

    klog_println("kernel: Initializing SystemX");
    XK_InitializeScheduler(&SystemX);

    /* _glob_tasks[0] is a reference to RootTask */

    #ifdef __Kernel_Print_Log_Then_Halt__ 
        /* For debugging the logs */
        asm("hlt");
    #endif

    clear();

    /* Trying a log-in*/

    XK_PrintTimeAndDate();
    XK_TryLogin(&logged_in, 1);

    while (1 == XK_IsRunning(0)) {
        /* Steps one tick */
        XK_SystemTick();
        XK_StepTasks();
    }

    klog_println("kernel: Kernel stopped.");

    /* Stops interrupts, then halts and has a for loop at the end */

    asm("cli");
    asm("hlt");
    for(;;){}
}
