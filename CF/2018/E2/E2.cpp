#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 2>> segs(N);
		for (int z = 0; z < 2; z++) {
			for (int i = 0; i < N; i++) {
				cin >> segs[i][z];
				segs[i][z] -= !z;
			}
		}
		const int V = 2*N;
		// sort by right endpoint
		std::sort(segs.begin(), segs.end(), [&](auto a, auto b) { return a[1] < b[1]; });
		std::vector<int> par(V, -1);
		auto reset = [&]() -> void {
			std::fill(par.begin(), par.end(), -1);
		};
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto solve = [&](int s) -> int {
			reset();
			for (auto [l, r] : segs) {
			}
		};
		int ans = 0;
		[&](this auto&& self, int l, int vl, int r, int vr) -> void {
			assert(l < r);
			assert(vl >= vr);
			if (vl == vr) return;
			if (r - l == 1) return;
			int m = (l + r) / 2;
			int vm = solve(m);
			ans = std::max(ans, m * vm);
			self(l, vl, m, vm);
			self(m, vm, r, vr);
		}(0, N+1, N+1, 0);
		cout << ans << '\n';
	}

	return 0;
}
