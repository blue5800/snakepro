#include <stdlib.h>

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
