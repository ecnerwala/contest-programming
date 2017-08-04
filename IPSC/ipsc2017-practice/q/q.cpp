#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
int N;
int E;

int v[MAXN];

int main() {
    ios_base::sync_with_stdio(0);
    int T; cin >> T;
    for (int t = 0; t < T; t++) {
        cin >> N >> E;
        for (int i = 1; i <= N; i++) {
            v[i] = -i;
        }
        for (int i = 1; i <= E; i++) {
            int j; cin >> j;
            v[j] = i;
        }
        int res = 1;
        for (int i = 2; i <= N; i++) {
            if (v[i] < v[res]) res = i;
        }
        cout << res << '\n';
    }

}
