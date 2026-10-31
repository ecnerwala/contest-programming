#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		std::vector<int> par(N, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		int num_ccs = N;
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a), b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			num_ccs--;
			return true;
		};

		std::vector<bool> is_used(2*N+1);
		auto solve = [&](std::vector<int> inds) {
			int C = int(inds.size());
			std::vector<int> pref(C);
			// pref[c] is the value right before inds[c]
			for (int c = 0; c < C; c++) {
				is_used[N + 2 * c - inds[c]] = true;
			}
			std::vector<int> pref_max = pref;
			for (int c = C-2; c >= 0; c--) {
				pref_max[c] = std::max(pref_max[c], pref_max[c+1]);
			}
			int cur = N;
			for (int i = 0, c = 0; i < N; i++) {
				if (c < C && inds[c] == i) {
					// just use this
					assert(cur == pref[c]);
					cur++;
				} else if (is_used[cur-1]) {
					// use this
					cur--;
				} else {
					// skip til the next used one
				}
			}
			for (int c = 0; c < C; c++) {
				is_used[N + 2 * c - inds[c]] = false;
			}
		};

		std::vector<std::vector<int>> inds_by_value(N);
		for (int i = 0; i < N; i++) {
			inds_by_value[A[i]].push_back(i);
		}
		for (int v = 0; v < N; v++) {
			solve(std::move(inds_by_value[v]));
		}
		cout << num_ccs << '\n';
	}

	return 0;
}
