#include <bits/stdc++.h>

int main() {
    using namespace std;
    const int N = 1e7;
    mt19937 mt(48);
    for (int i = 0; i < N; i++) {
        putc(uint8_t(mt() % 256), stdout);
    }
}
