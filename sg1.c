#include <sg1.h>
#include <stdint.h>
#include <string.h>

// SG1 - Stream Generator 1

// Rotate 16 byte state left by 7 bytes
// This helps diffuse the state
void memswap_7(char *buffer)
{
    char temp[7];
    memcpy(temp, buffer, 7);
    memmove(buffer, buffer + 7, 9);
    memcpy(buffer + 9, temp, 7);
}

// XOR Shift generator
void sg1_lsfr_x1(char *state_x1)
{
    // Performs a "sg1" LSFR algorithm on a 16-byte state.

    // Convert the state pointer to two uint64_t pointers for easier manipulation.
    uint64_t *s_0 = (uint64_t *)state_x1;
    uint64_t *s_1 = (uint64_t *)state_x1 + 1;

    // Apply XOR shift operations on the first 64-bit value in the state.
    *s_0 ^= *s_0 >> 12;
    *s_0 ^= *s_0 << 25;
    *s_0 ^= *s_0 >> 27;

    // Apply XOR shift operations on the second 64-bit value in the state.
    *s_1 ^= *s_1 >> 12;
    *s_1 ^= *s_1 << 25;
    *s_1 ^= *s_1 >> 27;

    // Update the state by XORing the two 64-bit values together.
    *s_0 ^= *s_1;
}

// Linear congruential generator 1
void sg1_lsfr_x2(char *state_x2)
{
    // Performs a "sg1" LSFR algorithm on a 16-byte state.

    // Convert the state pointer to two uint64_t pointers for easier manipulation.
    uint64_t *s_0 = (uint64_t *)state_x2;
    uint64_t *s_1 = (uint64_t *)state_x2 + 1;

    // Linear congruential generator 1 
    *s_0 = 2862933555777941757 * *s_0 + 3037000493;

    // Linear congruential generator 2
    *s_1 = 2862933555666941755 * *s_1 + 3037000493;
}

// Linear congruential generator 2
void sg1_lsfr_x3(char *state_x3)
{
    // Performs a "sg1" LSFR algorithm on a 16-byte state.

    // Convert the state pointer to two uint64_t pointers for easier manipulation.
    uint64_t *s_0 = (uint64_t *)state_x3;
    uint64_t *s_1 = (uint64_t *)state_x3 + 1;

    // Linear congruential generator 1
    *s_0 = 2863933555777941757 * *s_0 + 3037000493;

    // Linear congruential generator 2
    *s_1 = 2863933555666941755 * *s_1 + 3037000493;
}

// XOR Shift generator 2 with non-linear feedback
void sg1_lsfr_x4(char *state_x4)
{
    // Performs a "sg1" LSFR algorithm on a 16-byte state.

    // Convert the state pointer to two uint64_t pointers for easier manipulation.
    uint64_t *s_0 = (uint64_t *)state_x4;
    uint64_t *s_1 = (uint64_t *)state_x4 + 1;

    // Apply XOR shift operations on the first 64-bit value in the state.
    *s_0 ^= *s_0 >> 12;
    *s_0 ^= *s_0 << 25;
    *s_0 ^= *s_0 >> 27;

    // Apply XOR shift operations on the second 64-bit value in the state.
    *s_1 ^= *s_1 >> 12;
    *s_1 ^= *s_1 << 25;
    *s_1 ^= *s_1 >> 27;

    // Compute temporary variables by adding the two 64-bit values together.
    uint64_t t_0 = *s_0 + *s_1;
    uint64_t t_1 = *s_0 + *s_1;

    // Update the state by XORing the temporary variables with the original values.
    *s_0 ^= t_0;
    *s_1 ^= t_1;
}

void sg1_squeeze(struct SG1_state *state, char *buffer)
{
    uint64_t sq0[4];

    sq0[0] = *(uint64_t *)state->state[0];
    sq0[1] = *(uint64_t *)state->state[1];
    sq0[2] = *(uint64_t *)state->state[2];
    sq0[3] = *(uint64_t *)state->state[3];

    sq0[0] ^= ((uint64_t *)state->state[0])[1];
    sq0[1] ^= ((uint64_t *)state->state[1])[1];
    sq0[2] ^= ((uint64_t *)state->state[2])[1];
    sq0[3] ^= ((uint64_t *)state->state[3])[1];

    memcpy(buffer, sq0, 32);
}

void sg1_update_state(struct SG1_state *state)
{
    for (int i = 0; i < 1; i++)
    {
        memswap_7(state->state[0]);
        memswap_7(state->state[1]);
        memswap_7(state->state[2]);
        memswap_7(state->state[3]);

        // Update each state
        sg1_lsfr_x1(state->state[0]);
        sg1_lsfr_x2(state->state[1]);
        sg1_lsfr_x3(state->state[2]);
        sg1_lsfr_x4(state->state[3]);
    }
}

void sg1_derive_state(struct SG1_state *state, const char *key)
{
    // Initialize state with key
    memcpy(state->state[0], key, 16);
    memcpy(state->state[1], key, 16);
    memcpy(state->state[2], key, 16);
    memcpy(state->state[3], key, 16);

    // Update state 16 times
    for (int i = 0; i < 16; i++)
    {
        sg1_update_state(state);
    }
}

void sg1_drbg_gen(void *data, size_t size, struct SG1_state *state)
{
    char *buffer = (char *)data;

    while (size > 0)
    {
        sg1_squeeze(state, buffer);
        sg1_update_state(state);
        buffer += 32;
        size -= 32;
    }
}