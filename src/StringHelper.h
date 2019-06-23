#ifndef string_helper_h
#define string_helper_h

#include "Arduino.h"

class StringHelper {
public:
	static String split(String string, char separator, int index);
};

#endif /* string_helper_h */
