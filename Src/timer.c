
#include "stm32f4xx_ll_tim.h"
#include "timer.h"

struct timer_t
{
    TIM_TypeDef * timx;
};

static struct timer_t timer_instances[NUMBER_OF_TIMER_INSTANCES];
static uint8_t timer_instance_id = 0;

struct timer_t * timer_init(uint32_t address)
{
    struct timer_t * self = &timer_instances[timer_instance_id++];

    self->timx = (TIM_TypeDef *)address;

    return self;
}

void timer_set_prescaler(struct timer_t * self, uint32_t prescaler)
{
    LL_TIM_SetPrescaler(self->timx, prescaler);
}

void timer_set_period(struct timer_t * self, uint32_t period)
{
    LL_TIM_SetAutoReload(self->timx, period);
}

void timer_set_counter(struct timer_t * self, uint32_t count)
{
    LL_TIM_SetCounter(self->timx, count);
}

void timer_enable(struct timer_t * self)
{
    LL_TIM_EnableCounter(self->timx);
}

void timer_disable(struct timer_t * self)
{
    LL_TIM_DisableCounter(self->timx);
}

bool timer_is_update_set(struct timer_t * self)
{
    return (LL_TIM_IsActiveFlag_UPDATE(self->timx) == 1UL);
}

void timer_clear_update(struct timer_t * self)
{
    LL_TIM_ClearFlag_UPDATE(self->timx);
}
