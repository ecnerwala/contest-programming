#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	int K; cin >> K;
	vector<int> V(N);
	for (int i = 0; i < N; i++) cin >> V[i];

	int ans = 0;
	for (int l = 0; l <= N && l <= K; l++) {
		for (int r = 0; l + r <= N && l+r <= K; r++) {
			vector<int> cur; cur.reserve(l+r);
			for (int i = 0; i < l; i++) cur.push_back(V[i]);
			for (int i = 0; i < r; i++) cur.push_back(V[N-1-i]);

			sort(cur.begin(), cur.end());
			reverse(cur.begin(), cur.end());
			int ops = K - l - r;
			while (ops > 0 && !cur.empty() && cur.back() < 0) {
				--ops;
				cur.pop_back();
			}
			int cnd = 0;
			for (int v : cur) cnd += v;
			ans = max(ans, cnd);
		}
	}
	cout << ans << '\n';

	return 0;
}
