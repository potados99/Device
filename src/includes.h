#ifndef includes_h
#define includes_h

#include "Arduino.h"
#include "ILoop.h"
#include "StringHelper.h"

#define HAS(X, Y) ((X) & (Y))
#define ADD(X, Y) ((X) |= (Y))
#define SUB(X, Y) ((X) &= ~(Y))

#endif /* includes_h */
