#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	vector<vector<pair<int, int>>> small_ops(350);
	vector<int> ans(N);
	while (K--) {
		int A, L, D; cin >> A >> L >> D; A--;
		if (D < int(small_ops.size())) {
			small_ops[D].push_back({A, A+L*D});
		} else {
			for(; L--; A += D) ans[A]++;
		}
	}

	vector<int> cur(N);
	for (int d = 1; d < int(small_ops.size()); d++) {
		fill(cur.begin(), cur.end(), 0);
		for (auto& [a, b] : small_ops[d]) {
			cur[a]++;
			if (b < N) cur[b]--;
		}
		for (int i = 0; i < N; i++) {
			if (i >= d) cur[i] += cur[i-d];
			ans[i] += cur[i];
		}
	}
	for (int i = 0; i < N; i++) {
		cout << ans[i] << " \n"[i+1==N];
	}

	return 0;
}
