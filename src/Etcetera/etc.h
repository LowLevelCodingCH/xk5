#ifndef ETC_H_
#define ETC_H_

void halt() {
  while(1==1){
    asm("hlt");
  }
}

#define bool _Bool

typedef struct {
  bool locked;
} Spinlock;

#define true 1
#define false 0
#define __SPINLOCK(name) static Spinlock name = {.locked = false};

void unlock(Spinlock* spinlock) {
  spinlock->locked = false;
}

void lock(Spinlock* spinlock) {
  spinlock->locked = true;
}

void spinlock(Spinlock* spinlock) {
  while(spinlock->locked)
    ;
  spinlock->locked = true;
}


typedef unsigned int pba_t;
              // Physical block address

pba_t balloc(unsigned int size, unsigned int address) {
  if(address > size) return address - size;
  kpanicln("ERROR: BLOCK ALLOCATION UNDERFLOW", 1);
  return -100000000;
}

// pba_t intarray = balloc(sizeof(int) * 5, 200);
// intarray == 195; // true

#endif // ETC_H_
