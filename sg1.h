#pragma once

#include <stddef.h>

struct SG1_state
{
    char state[4][16];
};

void sg1_drbg_gen(void *data, size_t size, struct SG1_state *state);

void sg1_derive_state(struct SG1_state *state, const char *key);