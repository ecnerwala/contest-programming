#include <bits/stdc++.h>
using namespace std;

const int MAXN = 5e5;
int N;
multiset<int> A;

int main() {
    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    cin >> N;
    for (int i = 0; i < N; i++) {
        int a; cin >> a; A.insert(a);
    }
    int res = 0;
    while (!A.empty()) {
        auto it = A.end();
        it--;
        int v = *it;
        A.erase(it);
        int i = 0;
        while (v >= (1ll << i)) i++;
        assert(v < (1ll << i));
        int o = int((1ll << i) - v);
        assert(o <= v);
        auto jt = A.find(o);
        if (jt == A.end()) continue;
        res++;
        A.erase(jt);
    }
    cout << res << '\n';
    return 0;
}
