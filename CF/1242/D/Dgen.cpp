#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    const int K = 1e6;
    ofstream inf("t1e6.in");
    ofstream outf("t1e6.out");
    int T = (int(1e5) + K) / (K+1) * (K+1);
    inf << T << '\n';
    set<ll> seen;
    ll cur = 0;
    for (int t = 1; t <= T; ) {
        int cnt = 0;
        ll sum = 0;
        while (cnt < K) {
            if (!seen.count(++cur)) {
                inf << cur << ' ' << K << '\n';
                outf << t << '\n';
                cnt ++;
                sum += cur;
                t++;
            }
        }
        inf << sum << ' ' << K << '\n';
        outf << t << '\n';
        t++;
        seen.insert(sum);
    }
}
