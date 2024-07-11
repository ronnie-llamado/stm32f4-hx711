#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdbool.h>

extern bool hx711_data_is_ready;
extern bool toggle_led_flag;

void setup(void);
void loop(void);

#endif // PROGRAM_H