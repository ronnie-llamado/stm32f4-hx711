#include "exti.h"
#include "stm32f4xx_ll_exti.h"

struct exti_t
{
    uint32_t port;
    uint32_t pin;
};

static struct exti_t exti_instances[NUMBER_OF_EXTI_INSTANCES];
static uint8_t exti_instance_id = 0;

struct exti_t * exti_init(uint32_t port, uint32_t pin, enum exti_edge_t edge)
{
    LL_EXTI_InitTypeDef EXTI_InitStruct = {0};

    struct exti_t * self = &exti_instances[exti_instance_id++];

    self->port = port;
    self->pin = (1UL << pin);

    uint32_t trigger;

    switch(edge)
    {
        default:
        case EXTI_EDGE_NONE:
            trigger = LL_EXTI_TRIGGER_NONE;
            break;

        case EXTI_EDGE_FALLING:
            trigger = LL_EXTI_TRIGGER_FALLING;
            break;

        case EXTI_EDGE_RISING:
            trigger = LL_EXTI_TRIGGER_RISING;
            break;
    }

    EXTI_InitStruct.Line_0_31 = self->pin;
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger = trigger;
    LL_EXTI_Init(&EXTI_InitStruct);

    return self;
}

void exti_enable(struct exti_t * self)
{
    LL_EXTI_EnableIT_0_31(self->pin);
}

void exti_disable(struct exti_t * self)
{
    LL_EXTI_DisableIT_0_31(self->pin);
}
