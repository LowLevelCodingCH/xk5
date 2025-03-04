/* Ticks the system, used by a ton of things */
void XK_Prefix(SystemTick)(void) {
    _glob_tick++;
}

/* Lists the tasks and TaskID (TID) */
void XK_Prefix(ListTasks)() {
    for(int i = 0; i < _glob_amount; i++) {
        kprint("TID: ");
        kprint(itoa(i));
        kprint(": ");
        kprintbuf(_glob_tasks[i]->name, 32);
        kputln(ssize(itoa(i)) + 32 + 7);
    }
}

/* Appends/Attaches a task */
void XK_Prefix(AppendTask)(Task *task) {
    if(_glob_amount < MAX_TASK_AMOUNT){
        _glob_tasks[_glob_amount] = task;
        _glob_amount++;
        klog_print("scheduler: Attached Task: ");
        kprintbuf(task->name, 32);
        kputln(ssize("scheduler: Attached Task: ") + 32 + 17);
        task->init();
    }
    else {
        klog_println("scheduler: Too many tasks. Kill one.");
        XK_Prefix(ListTasks)();
    }
}

/* Removes/Kills/Detaches a task */
void XK_Prefix(RemoveTask)(uint32_t tind) {
    if(_glob_tasks[tind]->exists != TASK_EXIST_FLAG) return;
    _glob_tasks[tind]->running = 0;
    
    _glob_tasks[tind]->exists = 0;

    klog_print("scheduler: Detached Task: ");
    kprintbuf(_glob_tasks[tind]->name, 32);
    kputln(17 + ssize("scheduler: Detached Task: ") + 32);

    for (int i = tind; i < _glob_amount; i++)
        _glob_tasks[i] = _glob_tasks[i + 1];
    _glob_amount--;
}

/* Steps one tick for the task (I know this code is hard to navigate, I'll likely make a video on this) */
void XK_Prefix(TaskStep)(uint32_t index) {
    if(_glob_tasks[index]->exists != TASK_EXIST_FLAG) return;
    _glob_tasks[index]->current_tick++;
    if(_glob_tasks[index]->length != TASK_FOREVER_FLAG &&
    _glob_tasks[index]->current_tick >= _glob_tasks[index]->length)
        XK_RemoveTask(index);
    _glob_tasks[index]->update();
}

/* Steps one tick for all the tasks */
void XK_Prefix(StepTasks)() {
    for(int i = 0; i < _glob_amount; i++)
        XK_TaskStep(i);
}

/* Initializes the scheduler with the root task */
void XK_Prefix(InitializeScheduler)(Task *roottask) {
    klog_println("scheduler: Initializing Root Task");
    XK_AppendTask(roottask);
}

/* Looks at a task and tells if it is running or not */
uint32_t XK_Prefix(IsRunning)(uint32_t index) {
    return _glob_tasks[index]->running;
}
