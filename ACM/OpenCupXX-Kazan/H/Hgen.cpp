#include <bits/stdc++.h>
using namespace std;

int main() {
    mt19937 mt(49);
    const int N = 100, Q = 20000;
    cout << N << ' ' << Q << '\n';
    for (int i = 0; i < N; i++) {
        cout << int(mt() % 200) - 50 << " \n"[i+1==N];
    }
    for (int q = 0; q < Q; q++) {
        int l = mt() % N, r = mt() % N;
        if (l > r) swap(l, r);
        int k = mt() % (r-l+1) + 1;
        cout << l+1 << ' ' << r+1 << ' ' << k << '\n';
    }
}
