#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr);
    int N = 10, M = 11;
    int Q = 10000;
    mt19937 mt(48);
    cout << N << ' ' << M << ' ' << Q << '\n';
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cout << char('A' + mt() % 3);
        }
        cout << '\n';
    }
    for (int q = 0; q < Q; q++) {
        int xlo = mt() % N, xhi = mt() % N;
        if (xlo > xhi) swap(xlo, xhi);
        int ylo = mt() % M, yhi = mt() % M;
        if (ylo > yhi) swap(ylo, yhi);
        cout << xlo+1 << ' ' << ylo+1 << ' ' << xhi+1 << ' ' << yhi+1 << '\n';
    }
}
