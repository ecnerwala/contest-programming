#include<bits/stdc++.h>
using namespace std;

int main() {
    int N = 99998, M = 99999, D = 50;
    cout << N << ' ' << M << ' ' << D << '\n';
    mt19937 mt(48);
    for (int i = 0; i < M; i++) {
        int u = (mt() % N + 1);
        int v = (mt() % N + 1);
        cout << u << ' ' << v << '\n';
    }
    for (int i = 0; i < N; i++) {
        for (int d = 0 ;d < D; d++) {
            cout << '1';
        }
        cout << '\n';
    }
}
