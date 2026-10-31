#include <bits/stdc++.h>
using namespace std;

int main() {
    int N = 1500;
    int M = 1500;
    cout << N << ' ' << M << '\n';
    cout << 0 << ' ' << 1 << '\n';
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (i == 0 && j == 0) cout << 'S';
            else if (i == N-1 && j == M-1) cout << 'E';
            else cout << '@';
        }
        cout << '\n';
    }
}
