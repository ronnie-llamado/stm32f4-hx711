#ifndef DIO_H
#define DIO_H

#include <stdint.h>

#define NUMBER_OF_DIO_INSTANCES 3

enum dio_direction_t
{
    DIO_DIRECITON_INPUT,
    DIO_DIRECTION_OUTPUT,
};

#define DIO_VALUE_OFF 0U
#define DIO_VALUE_ON 1U

struct dio_t;

struct dio_t * dio_init(uint32_t address, uint32_t pin, enum dio_direction_t direction);
void dio_write(struct dio_t * self, uint8_t value);
void dio_toggle(struct dio_t * self);
uint8_t dio_read(struct dio_t * self);

#endif // DIO_H