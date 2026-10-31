#include <bits/stdc++.h>
using namespace std;
int main() {
    for (int z = 0; z < 10; z++) {
        //int N = 12, Q = 12;
        int N = 2e5, Q = 2e5;
        const int X = 1e8;
        assert(N % 4 == 0);

        cout << N << ' ' << Q << '\n';
        cout << -X << ' ' << X << '\n';
        int L = N/4 - 1;
        for (int i = 0; i < L; i++) {
            cout << -X + i << ' ' << -X + L-i << '\n';
            cout << -X + i + 1 << ' ' << -X + L-i << '\n';
        }
        cout << -X + L << ' ' << -X << '\n';
        cout << X << ' ' << -X << '\n';
        for (int i = 0; i < L; i++) {
            cout << X - i << ' ' << X - (L-i) << '\n';
            cout << X - (i + 1) << ' ' << X - (L-i) << '\n';
        }
        cout << X - L << ' ' << X << '\n';

        for (int q = 0; q < Q; q++) {
            cout << q << ' ' << -q << '\n';
        }

    }
}
