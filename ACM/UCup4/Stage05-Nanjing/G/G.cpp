#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> V(N);
		for (auto& v : V) cin >> v;
		std::vector<int> L(N);
		for (auto& l : L) cin >> l;
		std::sort(V.begin(), V.end());
		std::sort(L.begin(), L.end(), std::greater<>());
		int Q; cin >> Q;
		std::vector<std::pair<int, int>> queries(Q);
		for (auto& [t, _] : queries) cin >> t;
		for (int q = 0; q < Q; q++) queries[q].second = q;
		std::sort(queries.begin(), queries.end(), std::greater<>());

		std::vector<int64_t> ans(Q);
		int64_t tot_V = 0;
		int64_t tot_L = 0;
		for (auto [t, q] : queries) {
			while (!L.empty() && int64_t(L.back()) * t < int64_t(V.back())) {
				tot_V += V.back(); V.pop_back();
				tot_L += L.back(); L.pop_back();
			}
			ans[q] = tot_V - t * tot_L;
		}
		for (int q = 0; q < Q; q++) cout << ans[q] << " \n"[q+1==Q];
	}

	return 0;
}
