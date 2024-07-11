#include "maf.h"

static struct maf_t maf_instances[NUMBER_OF_MAF_INSTANCES];
static uint8_t maf_instance_id = 0;

struct maf_t * maf_init(void)
{
    struct maf_t * self = &maf_instances[maf_instance_id++];

    for(uint8_t i = 0; i < MAF_WINDOW_LENGTH; i++)
    {
        self->buffer[i] = 0;
    }

    self->sum = 0;
    self->average = 0;

    return self;
}

void maf_add(struct maf_t * self, int32_t value)
{
    self->sum -= self->buffer[self->head];
    self->sum += value;

    self->buffer[self->head] = value;
    self->head++;

    if (self->head == MAF_WINDOW_LENGTH)
    {
        self->head = 0;
    }

    self->average = self->sum / MAF_WINDOW_LENGTH;
}

int32_t maf_get_average(struct maf_t * self)
{
    return self->average;
}
