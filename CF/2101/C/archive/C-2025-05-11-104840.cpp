#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N); for (auto& a : A) { cin >> a; }

		int64_t ans = 0;
		std::vector<int> locs; locs.reserve(N);
		for (int z = 0; z < 2; z++, std::reverse(A.begin(), A.end()), std::reverse(locs.begin(), locs.end())) {
			std::vector<int> par(N+1, -1);
			auto get_par = [&](int a) -> int {
				while (par[a] >= 0) {
					if (par[par[a]] >= 0) par[a] = par[par[a]];
					a = par[a];
				}
				return a;
			};
			std::vector<int> label(N+1); std::iota(label.begin(), label.end(), 0);
			auto merge = [&](int a, int b) -> bool {
				a = get_par(a), b = get_par(b);
				if (a == b) return false;
				label[b] = label[a];
				if (par[a] > par[b]) std::swap(a, b);
				par[a] += par[b];
				par[b] = a;
				return true;
			};
			for (int i = 0; i < N; i++) {
				int x = get_par(A[i]);
				if (label[x] > 0) {
					merge(x, label[x]-1);
					if (z) {
						if (locs.empty()) break;
						if (N - 1 - i > locs.back()) {
							ans += (N - 1 - i) - locs.back();
							locs.pop_back();
						} else {
							break;
						}
					} else {
						locs.push_back(i);
					}
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
