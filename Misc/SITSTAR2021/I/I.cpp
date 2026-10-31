#include <bits/stdc++.h>

int main() {
    using namespace std;
    ios_base::sync_with_stdio(false); cin.tie(nullptr);

    int N; cin >> N;
    vector<int> cnt_1(2*N);
    vector<int> cnt_all(2*N);
    for (int i = 0; i < N; i++) {
        int a, b; cin >> a >> b; a--, b--;
        if (a > b) swap(a, b); assert(a <= b);
        cnt_1[a]++;
        if (b != a) cnt_1[b]++;
        cnt_all[a]++;
        cnt_all[b]++;
    }
    nth_element(cnt_1.begin(), cnt_1.end()-2, cnt_1.end());
    int ans = 2 * N - max(cnt_1.end()[-2] + cnt_1.end()[-1], *max_element(cnt_all.begin(), cnt_all.end()));
    cout << ans << '\n';
}
