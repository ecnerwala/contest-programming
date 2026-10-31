#include <stdio.h>

static int clicks = 0;
static long long int emojis = 0;
static long long int clipboard = 0;

void auguri(long long int N);

void aggiungi() {
    clicks += 1;
    emojis += 1;
}

void copia() {
    clicks += 2;
    clipboard = emojis;
}

void incolla() {
    if (clipboard == 0) {
        fprintf(stderr, "Warning: it does not make sense to call 'incolla' before 'copia'.\n");
    }

    clicks += 1;
    emojis += clipboard;
}

int main() {
    long long int N;
    scanf("%lld", &N);

    auguri(N);

    if (emojis != N) {
        fprintf(stderr, "Warning: the message you built has %lld emojis instead of %lld.\n", emojis, N);
    }

    printf("%d\n", clicks);
}