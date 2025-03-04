/* Defines a 512 char long Users string */
char UsersStr[512] = {0};
char UserAmount = 0;

/* Space for 4 users */
User users[4] = {0};

/* Lists the users via the users struct and amount */
void XK_Prefix(ListUsers)(User _users[4], int _amount) {
    for(int i = 0; i < _amount; i++)
        kprintln(_users[i].name);
}

/* Initializes the User-System */
void XK_Prefix(InitUsers)(void) {
    klog_println("users: Reading users file");
    
    /* Reads the users file into a string */
    XK_ReadFile(FST_Entries, FST_Blocks, "XK$USERS", UsersStr);
    UserAmount = UsersStr[0];
    
    /* Errors if no user was found, then halts */
    if ((uint32_t)UserAmount == 0) {
        clear();
        klog_println("users: No user found");
        asm("hlt");
        return;
    }
    
    /* Gets the data for each user via for loops. This is O(n****n) surely. (Idk actually) */
    for(int i = 0; i < (uint32_t)UserAmount; i++) {
        for(int j = 0; j < 16; j++)
    	    users[i].name[j] = UsersStr[i*(16*2)+0+1+j];
        for(int j = 0; j < 16; j++)
            users[i].passcode[j] = UsersStr[i*(16*2)+16+1+j];
    }
    
    /* Logs if the users got initialized, then prints more data */
    klog_print("users: Initialized ");
    kprint(itoa((uint32_t)UserAmount));
    kprint(" users");
    kputln(17 + ssize("users: Initialized ") + ssize(itoa((uint32_t)UserAmount)) + 6);
}

/* Tries a Log-In and changes the variable *loggedin to match the state */
void XK_Prefix(TryLogin)(int *loggedin, int showtimeanddate) {
    while(0 == *loggedin) {
        if(1 == showtimeanddate) XK_PrintTimeAndDate();

        kprintln("----------------");
        /* Lists the users */
        XK_Prefix(ListUsers)(users, (uint32_t)UserAmount);
        
        kprintln("----------------");
        
        /* Gets credentials */
        kprint("Username: ");
        read_word(try_username, 16, 0, 0);
        
        set_cursor(get_cursor() - (ssize("Username: ")*2));
        
        kprint("Passcode: ");
        read_word(try_passcode, 16, ssize("Username: ") + ssize(try_username), 0);
        
        if(1 == showtimeanddate) XK_PrintTimeAndDate();

        /* Looks up the user that the user gave us (confusing I know) */
        for(int i = 0; i < (uint32_t)UserAmount; i++) {
            if(streq(try_username,users[i].name)==0 &&
            streq(try_passcode,users[i].passcode)==0) {
                for(int j = 0; j < 16; j++)
                    user[j] = try_username[j];
                *loggedin = 1;
            }
        }
        clear();
        
        /* For memory safety and general happiness*/
        clearBuffer(try_username, 16);
        clearBuffer(try_passcode, 16);
    }
    
    /* Yet again */
    clearBuffer(try_username, 16);
    clearBuffer(try_passcode, 16);
    
    klog_print("Login: Logged in successfully as: ");
    kprint(user);
    kputln(ssize(user) + ssize("Login: Logged in successfully as: ") + 17);
}
