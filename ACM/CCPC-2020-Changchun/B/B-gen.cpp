#include <bits/stdc++.h>
using namespace std;

int main() {
    int N = 1e5;
    int M = 1e9;
    int Q = 1e5;
    mt19937 mt(48);
    vector<int> A(N);
    for (int i = 0; i < N; i++) {
        A[i] = uniform_int_distribution(1, M-1)(mt);
    }

    cout << N << ' ' << M << ' ' << Q << '\n';
    for (int i = 0; i < N; i++) {
        cout << A[i] << " \n"[i+1==N];
    }
    for (int q = 0; q < Q; q++) {
        if (mt() & 1) {
            int l = uniform_int_distribution(1, N-1)(mt);
            int r = uniform_int_distribution(1, N-1)(mt);
            if (l > r) swap(l, r);
            r++;
            int k = uniform_int_distribution(1, r-l)(mt);
            cout << 1 << ' ' << l << ' ' << r << ' ' << k << '\n';
        } else {
            int u = uniform_int_distribution(1, N)(mt);
            int v = uniform_int_distribution(1, M-1)(mt);
            cout << 2 << ' ' << u << ' ' << v << '\n';
        }
    }
}
