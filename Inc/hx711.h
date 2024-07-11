#ifndef HX711_H
#define HX711_H

#include <stdint.h>

#include "dio.h"
#include "timer.h"

#define NUMBER_OF_HX711_INSTANCES 1

struct hx711_t;

struct hx711_t * hx711_init(struct dio_t * data, struct dio_t * clock, struct timer_t * timer);
int32_t hx711_read_count(struct hx711_t * self);

#endif // HX711_H