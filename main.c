#include <sg1.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE (32 * 4096)

int main()
{

    char *buffer = (char *)malloc(BUFFER_SIZE);

    const char key[16] = "0000000000000001";
    FILE *fp = fopen("output", "wb");

    struct SG1_state state;
    sg1_derive_state(&state, key);

    int i = 0;
    size_t bytes_written = 0;
    while (i < BUFFER_SIZE)
    {
        sg1_drbg_gen(buffer + i, 32, &state);
        i += 32;

        if (i == BUFFER_SIZE)
        {
            printf("\r%f GB bytes written", bytes_written / 1000.0 / 1000.0 / 1000.0);
            fflush(stdout);
            fwrite(buffer, 1, BUFFER_SIZE, fp);
            i = 0;
            bytes_written += BUFFER_SIZE;
        }
    }
}