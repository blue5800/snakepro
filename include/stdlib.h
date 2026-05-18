#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define RAND_MAX 32767
#define RAND_MIN 0

void itoa(int value, char* buffer);
void itoh(int value, char* buffer);

void srand(unsigned int seed);

int abs(int value);

int rand();
