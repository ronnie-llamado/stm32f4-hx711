
#include "stm32f4xx_ll_gpio.h"
#include "dio.h"

struct dio_t
{
    // STM32
    GPIO_TypeDef * gpiox;
    uint32_t pin;
};

static struct dio_t dio_instances[NUMBER_OF_DIO_INSTANCES];
static uint8_t dio_instance_id = 0;

struct dio_t * dio_init(uint32_t address, uint32_t pin, enum dio_direction_t direction)
{
    struct dio_t * self = &dio_instances[dio_instance_id++];

    self->gpiox = (GPIO_TypeDef *)address;
    self->pin = (1UL << pin);

    uint32_t mode = (direction == DIO_DIRECTION_OUTPUT) ? LL_GPIO_MODE_OUTPUT : LL_GPIO_MODE_INPUT;

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = self->pin;
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

    LL_GPIO_Init(self->gpiox, &GPIO_InitStruct);

    return self;
}

inline void dio_write(struct dio_t * self, uint8_t value)
{
    switch(value)
    {
        case DIO_VALUE_OFF:
            LL_GPIO_ResetOutputPin(self->gpiox, self->pin);
            break;

        case DIO_VALUE_ON:
            LL_GPIO_SetOutputPin(self->gpiox, self->pin);
            break;

        default:
            break;
    }
}

inline void dio_toggle(struct dio_t * self)
{
    LL_GPIO_TogglePin(self->gpiox, self->pin);
}

inline uint8_t dio_read(struct dio_t * self)
{
    return LL_GPIO_IsInputPinSet(self->gpiox, self->pin);
}