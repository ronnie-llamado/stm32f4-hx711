#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>

#define NUMBER_OF_TIMER_INSTANCES 2

struct timer_t;

struct timer_t * timer_init(uint32_t address);
void timer_set_prescaler(struct timer_t * self, uint32_t prescaler);
void timer_set_period(struct timer_t * self, uint32_t period);
void timer_set_counter(struct timer_t * self, uint32_t count);
void timer_enable(struct timer_t * self);
void timer_disable(struct timer_t * self);
bool timer_is_update_set(struct timer_t * self);
void timer_clear_update(struct timer_t * self);

static inline void timer_delay(struct timer_t * self, uint32_t count)
{
    timer_set_period(self, count);
    timer_set_counter(self, 0);
    timer_clear_update(self);
    while(timer_is_update_set(self) != true);
}

#endif // TIMER_H