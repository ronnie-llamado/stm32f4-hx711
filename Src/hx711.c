#include <stdbool.h>

#include "dio.h"
#include "hx711.h"
#include "timer.h"

struct hx711_t
{
    struct dio_t * clock;
    struct dio_t * data;

    struct timer_t * timer;

    float full_scale_voltage;
    uint8_t gain;
};

struct hx711_t hx711_instances[NUMBER_OF_HX711_INSTANCES];
uint8_t hx711_instance_id = 0;

bool is_ready(struct hx711_t * self);
void enable(struct hx711_t * self);
void disable(struct hx711_t * self);

struct hx711_t * hx711_init(struct dio_t * data, struct dio_t * clock, struct timer_t * timer)
{
    struct hx711_t * self = &hx711_instances[hx711_instance_id++];

    self->data = data;
    self->clock = clock;
    self->timer = timer;
    self->gain = 128;

    enable(self);

    return self;
}

int32_t hx711_read_count(struct hx711_t * self)
{
    int32_t data = 0;

    for (uint8_t i = 0; i < 24; i++)
    {
        data <<= 1;

        dio_write(self->clock, DIO_VALUE_ON);
        timer_delay(self->timer, 1);

        if (dio_read(self->data) == 1UL)
        {
            data++;
        }

        timer_delay(self->timer, 1);
        dio_write(self->clock, DIO_VALUE_OFF);
        timer_delay(self->timer, 2);
    }

    dio_write(self->clock, DIO_VALUE_ON);
    timer_delay(self->timer, 2);
    dio_write(self->clock, DIO_VALUE_OFF);

    // Sign extend
    if (data & (1 << 23UL))
    {
        data |= (uint32_t)(0xFFFFFFFFUL << 24UL);
    }

    return data;
}

bool is_ready(struct hx711_t * self)
{
    return (dio_read(self->data) == 0u);
}

void enable(struct hx711_t * self)
{
    dio_write(self->clock, 0u);
}

void disable(struct hx711_t * self)
{
    dio_write(self->clock, 1u);
}