#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, K; cin >> N >> K;
    vector<int> S(N);
    for (int i = 0; i < N; i++) cin >> S[i];
    int Q; cin >> Q;
    while (Q--) {
        int op; cin >> op;
        if (op == 1) {
            int i, t; cin >> i >> t; i--;
            S[i] = t;
        } else if (op == 2) {
            int l, r; cin >> l >> r; l--, r--;
            bool good = true;
            vector<int> stk;
            for (int i = l; i <= r; i++) {
                if (S[i] > 0) {
                    stk.push_back(S[i]);
                } else {
                    if (stk.empty() || stk.back() != -S[i]) {
                        good = false;
                        break;
                    } else {
                        stk.pop_back();
                    }
                }
            }
            if (!stk.empty()) good = false;
            cout << (good ? "Yes" : "No") << '\n';
        }
    }
}
