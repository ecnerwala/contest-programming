#include "grader.h"

#include <bits/stdc++.h>

int MEM[5500];
int get(int index) {
    assert(0 <= index && index < 5500);
    return MEM[index];
}
void set(int index, int value) {
    assert(0 <= index && index < 5500);
    MEM[index] = value;
}
void shoutMinimum(int output) {
    printf("%d\n", output);
}

const int MAXN = 2e6;
int N, K;
int C[MAXN];

int getTrainLength() {
    return N;
}
int getWindowLength() {
    return K;
}

int ind;
int pass;
int getCurrentCarIndex() {
    return ind;
}
int getCurrentPassIndex() {
    return pass;
}

extern void helpBessie(int ID);

void run() {
    for (pass = 0; pass < 2; pass++) {
        for (ind = 0; ind < N; ind++) {
            helpBessie(C[ind]);
        }
    }
}


int main() {
    scanf("%d %d", &N, &K);
    for (int i = 0; i < N; i++) {
        scanf("%d", &C[i]);
    }
    run();
}
