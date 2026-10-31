#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    mt19937_64 mt(58);
    const int D = 2, M = 60;
    cout << D << ' ' << M << '\n';
    for (int i = 0; i < D; i++) {
        cout << (i == 0 ? 0 : (mt() % M)) << " \n"[i+1==D];
    }
    const int N = 30000;
    cout << N << '\n';
    for (int i = 0; i < N; i++) {
        cout << (mt() % M) << " \n"[i+1==N];
    }
    cout << ll(0.5e18) << ' ' << ll(1e18) << '\n';

    return 0;
}
