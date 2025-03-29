#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		int cnt_free = 0;
		std::vector<int> consts; consts.reserve(N);
		std::vector<std::pair<int, int>> pts; pts.reserve(N);
		{
			std::vector<int> M(N); for (auto& m : M) cin >> m;
			std::vector<int> C(N); for (auto& c : C) cin >> c;
			for (int i = 0; i < N; i++) {
				if (C[i] == 0) cnt_free++;
				else if (M[i] == 0) consts.push_back(C[i]);
				else pts.push_back({M[i], C[i]});
			}
		}
		std::vector<int> queries(Q);
		for (auto& q : queries) cin >> q;
		int max_query = *std::max_element(queries.begin(), queries.end());

		std::sort(pts.begin(), pts.end(), [&](auto a, auto b) {
			return int64_t(a.first) * int64_t(b.second) > int64_t(b.first) * int64_t(a.second);
		});
		std::vector<int> min_cost; min_cost.reserve(35);
		min_cost.push_back(0);
		for (auto [m, c] : pts) {
			for (int l = int(min_cost.size()) - 1; l >= 0; l--) {
				int64_t cnd_ = int64_t(min_cost[l]) * int64_t(m+1) + c;
				if (cnd_ > max_query) {
					continue;
				}
				int cnd = int(cnd_);
				if (l+1 == int(min_cost.size())) min_cost.push_back(int(cnd));
				else if (cnd < min_cost[l+1]) min_cost[l+1] = cnd;
			}
		}
		std::sort(consts.begin(), consts.end());
		std::vector<int> ans(N+1, max_query + 1);
		for (int i = 0; i <= cnt_free; i++) ans[i] = 0;
		int pref = 0;
		for (int i = 0; i <= int(consts.size()); i++) {
			if (i) pref += consts[i-1];
			if (pref > max_query) break;
			for (int j = 0; j < int(min_cost.size()); j++) {
				ans[i+j+cnt_free] = std::min(ans[i+j+cnt_free], pref + min_cost[j]);
			}
		}
		for (int q = 0; q < Q; q++) {
			int p = queries[q];
			cout << int(std::upper_bound(ans.begin(), ans.end(), p) - ans.begin()) - 1 << " \n"[q+1==Q];
		}
	}

	return 0;
}
