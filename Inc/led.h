#ifndef LED_H
#define LED_H

#include "dio.h"
#include "timer.h"

#define NUMBER_OF_LED_INSTANCES 1

struct led_t
{
    struct dio_t * dio;
    struct timer_t * timer;
};

struct led_t * led_init(struct dio_t * dio, struct timer_t * timer);
void led_process(struct led_t * self);

#endif // LED_H