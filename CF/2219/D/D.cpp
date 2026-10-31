#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> P(N); for (auto& p : P) { std::cin >> p; }

		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; std::cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		std::vector<int> st(N);
		std::vector<int> en(N);
		{
			int idx = 0;
			[&](this auto&& self, int cur, int prv) -> void {
				st[cur] = idx++;
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					self(nxt, cur);
				}
				en[cur] = idx;
			}(0, -1);
			assert(idx == N);
		}
		std::vector<int> invP(N);
		for (int i = 0; i < N; i++) invP[P[i]] = i;

		int cur_st = 0, cur_en = N;
		int cur_mex = 0;

		while (cur_st < cur_en) {
			while (cur_mex < N && st[invP[cur_mex]] <= cur_st && cur_en <= en[invP[cur_mex]]) {
				cur_mex++;
			}

			// find the next guys

			{
				// increment cur_mex
				if (cur_mex == N) break;
				cur_st = std::max(cur_st, st[invP[cur_mex]]);
				cur_en = std::min(cur_en, en[invP[cur_mex]]);
				cur_mex++;
			}
		}
	}

	return 0;
}
