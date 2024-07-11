#ifndef EXTI_H
#define EXTI_H

#include <stdint.h>

#define NUMBER_OF_EXTI_INSTANCES 1

#define LL_EXTI_TRIGGER_NONE            ((uint8_t)0x00U) /*!< No Trigger Mode */
#define LL_EXTI_TRIGGER_RISING          ((uint8_t)0x01U) /*!< Trigger Rising Mode */
#define LL_EXTI_TRIGGER_FALLING         ((uint8_t)0x02U) /*!< Trigger Falling Mode */
#define LL_EXTI_TRIGGER_RISING_FALLING  ((uint8_t)0x03U) /*!< Trigger Rising & Falling Mode */

enum exti_edge_t
{
    EXTI_EDGE_NONE,
    EXTI_EDGE_RISING,
    EXTI_EDGE_FALLING,
};

struct exti_t;

struct exti_t * exti_init(uint32_t port, uint32_t pin, enum exti_edge_t edge);
void exti_enable(struct exti_t * self);
void exti_disable(struct exti_t * self);

#endif // EXTI_H