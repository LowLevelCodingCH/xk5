#ifndef KEYVAL_H_
#define KEYVAL_H_

#define bool  _Bool
#define true  1
#define false 0

bool keyval(char* key) {
  if (ssize(key) != 9)
    return false;
  if(streq(key, "JQF541ZX8") == 0)
    return true;
  else if(streq(key, "928FA134Q") == 0)
    return true;
  else if(streq(key, "919000192") == 0)
    return true;
  else if(streq(key, "JQFDASFTR") == 0)
    return true;
  return false;
}


// Key Format:
// XXX-YYY-ZZZ
// Written:
// XXXYYYZZZ

// ___ ___ _____
// XXX-YYY-ZZZ
// XXX = Product ID
// YYY = Product Value
// ZZZ = Random String
#endif // KEYVAL_H_
