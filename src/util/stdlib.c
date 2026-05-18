#include <stdlib.h>
#include <clock.h>

void itoa(int value, char* buffer) {
    int i = 0;
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }
    while (value > 0 && i < 31) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }
    buffer[i] = '\0';
    // reverse the string
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
}

void itoh(int value, char* buffer) {
    int i = 0;
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }
    while (value > 0 && i < 31) {
        int digit = value % 16;
        buffer[i++] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
        value /= 16;
    }
    buffer[i] = '\0';
    // reverse the string
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
}

int srand_seed;

void srand(unsigned int seed) {
    // simple linear congruential generator
    srand_seed = seed;
}

int rand() {
    srand_seed = (srand_seed * 1103515245 + 12345) & 0x7fffffff;
    return (srand_seed % (RAND_MAX - RAND_MIN + 1)) + RAND_MIN;
}

int abs(int value){
    return (value < 0) ? -value : value;
}
