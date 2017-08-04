#include <bits/stdc++.h>
using namespace std;

long long go() {
    int N; cin >> N;
    long long cnt = 1;
    long long res = 1;
    for (int i = 1; i < N; i++) {
        long long a; cin >> a;
        cnt *= a;
        res += cnt;
    }
    cerr << N << ' ' << res << ' ' << cnt << '\n';
    return res;
}

int main() {
    ios_base::sync_with_stdio(0);
    int T; cin >> T;
    for (int t = 0; t < T; t++) {
        long long res = go();
        cout << res << '\n';
    }
}
