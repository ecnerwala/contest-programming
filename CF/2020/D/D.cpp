#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<int> par(N, -1);
		int comps = N;
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a), b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			comps--;
			return true;
		};
		std::vector<std::vector<std::pair<int, int>>> updates(11);
		for (int q = 0; q < M; q++) {
			int a, d, k; cin >> a >> d >> k;
			a--;
			updates[d].push_back({a, a + k * d});
		}
		for (int d = 1; d <= 10; d++) {
			std::vector<int> pref(N);
			for (auto [x, y] : updates[d]) pref[x]++, pref[y]--;
			for (int i = 0; i < N; i++) {
				assert(pref[i] >= 0);
				if (pref[i]) {
					assert(i + d < N);
					merge(i, i + d);
					pref[i+d] += pref[i];
				}
			}
		}
		cout << comps << '\n';
	}
	return 0;
}
