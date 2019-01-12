#ifndef _includes_h
#define _includes_h

#include "Arduino.h"
#include "iloop.h"

#define HAS(X, Y) ((X) & (Y))
#define ADD(X, Y) ((X) |= (Y))
#define SUB(X, Y) ((X) &= ~(Y))

#endif /* _includes_h */
