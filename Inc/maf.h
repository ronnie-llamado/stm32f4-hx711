#ifndef MAF_H
#define MAF_H

#include <stdint.h>
#include <stdlib.h>

#define NUMBER_OF_MAF_INSTANCES 1
#define MAF_WINDOW_LENGTH 32

struct maf_t
{
    int32_t buffer[MAF_WINDOW_LENGTH];

    size_t head;

    int32_t sum;
    int32_t average;
};

struct maf_t * maf_init(void);
void maf_add(struct maf_t * self, int32_t value);
int32_t maf_get_average(struct maf_t * self);

#endif // MAF_H