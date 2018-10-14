#include <Arduino.h>
#include "do-something.h"
int doSomething(void) {
  millis();  // this line is only here to test that we're able to refer to the builtins
  uint8_t i = 1;
  uint8_t j = 1;
  uint8_t k = 2;
  return (i + j + k);
};
