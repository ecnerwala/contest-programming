#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> C(N);
		for (auto& c : C) cin >> c;
		for (int k = 0; k < K; k++) {
			int p; cin >> p; p--;
			C[p] = 0;
		}

		std::vector<std::vector<int>> mixes(N);
		std::vector<int> out_deg(N);
		std::vector<std::vector<int>> in_edges(N);
		for (int i = 0; i < N; i++) {
			int l; cin >> l;
			mixes[i].resize(l);
			for (auto& j : mixes[i]) {
				cin >> j; j--;
				in_edges[j].push_back(i);
				out_deg[i]++;
			}
		}
		std::vector<int> q; q.reserve(N);
		for (int i = 0; i < N; i++) {
			if (!out_deg[i]) q.push_back(i);
		}
		std::vector<int64_t> cur_val(N, 0);
		for (int z = 0; z < int(q.size()); z++) {
			int i = q[z];
			if (!mixes[i].empty() && cur_val[i] < C[i]) C[i] = int(cur_val[i]);
			for (int j : in_edges[i]) {
				out_deg[j]--;
				cur_val[j] += C[i];
				if (!out_deg[j]) {
					q.push_back(j);
				}
			}
		}
		assert(int(q.size()) == N);
		for (int i = 0; i < N; i++) {
			cout << C[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
