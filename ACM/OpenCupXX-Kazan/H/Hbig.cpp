#include <bits/stdc++.h>
using namespace std;

int main() {
    mt19937 mt(49);
    int N = 35000, Q = 35000;
    cout << N << ' ' << Q << '\n';
    for (int i = 0; i < N; i++) {
        cout << int(mt() % 40000) - 5000 << " \n"[i+1==N];
    }
    for (int q = 0; q < Q; q++) {
        int l = mt() % N, r = mt() % N;
        if (l > r) swap(l, r);
        int k = mt() % (r-l+1) + 1;
        cout << l+1 << ' ' << r+1 << ' ' << k << '\n';
    }
}
