
#include <stdint.h>
#include "led.h"

static struct led_t led_instances[NUMBER_OF_LED_INSTANCES];
static uint8_t led_instance_id = 0;

struct led_t * led_init(struct dio_t * dio, struct timer_t * timer)
{
    struct led_t * self = &led_instances[led_instance_id++];
    self->dio = dio;
    self->timer = timer;
    return self;
}

void led_process(struct led_t * self)
{
    if (timer_is_update_set(self->timer) == true)
    {
        timer_clear_update(self->timer);
        dio_toggle(self->dio);
    }
}
