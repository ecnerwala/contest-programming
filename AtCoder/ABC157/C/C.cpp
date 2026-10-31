#include<bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int N, M; cin >> N >> M;
    vector<pair<int, char>> constraints(M);
    for (auto& it : constraints) cin >> it.first >> it.second;

    for (int i = 0; i < 10000; i++) {
        string s = to_string(i);
        if (s.size() != N) continue;
        bool good = true;
        for (auto it : constraints) {
            good = good && s[it.first-1] == it.second;
        }
        if (good) {
            cout << i << '\n';
            exit(0);
        }
    }
    cout << -1 << '\n';
}
