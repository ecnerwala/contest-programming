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
				segs[i][z] --;
			}
		}
		const int V = 2*N;
		// sort by right endpoint
		std::sort(segs.begin(), segs.end(), [&](auto a, auto b) { return a[1] < b[1]; });
		std::vector<int> par(V, -1);
		// first is linked list pointer, second is actual value
		std::vector<std::pair<int, int>> val(V, {0, 0});
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
			cerr << "solve " << s << '\n';
			reset();
			int cnd = 0;

			int cur_l = 0;
			int cur_r = -1;
			for (auto [l, r] : segs) {
				cerr << "process " << l << ' ' << r << '\n';
				if (l < cur_l) continue;
				if (cur_r < r) {
					val[r] = {cur_r, 0};
					par[r] = -(r - cur_r);
					// merge everything into r
					while ((++cur_r) < r) par[cur_r] = r;
				}
				l = get_par(l);
				val[l].second++;
				if (val[l].first >= cur_l) {
					int o = val[l].first = get_par(val[l].first);
					cerr << o << ' ' << l << ' ' << val[o].second << ' ' << val[l].second << '\n';
					assert(val[o].second >= val[l].second);
					if (val[o].second == val[l].second) {
						val[l].first = val[o].first;
						if (par[l] > par[o]) std::swap(l, o);
						par[l] += par[o];
						par[o] = l;
					}
				} else {
					if (val[l].second == s) {
						// move on
						cur_l = cur_r+1;
						cnd++;
					}
				}
			}
			return cnd;
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
