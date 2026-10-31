#include<bits/stdc++.h>
using namespace std;

int main() {
    int N = 2000, M = 2000;
    cout << N << ' ' << M << '\n';
    for (int i = 1; i < N; i++) {
        cout << i << ' ' << i+1 << '\n';
    }
    for (int i = 0; i < M; i++) {
        cout << i+1 << ' ' << (i+1)%N + 1 << '\n';
    }
}
