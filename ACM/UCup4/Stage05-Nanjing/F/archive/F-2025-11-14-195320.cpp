#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		const int B = 12;
		const int V = 1 << B;
		std::vector<std::vector<int>> pars(V, std::vector<int>(N, -1));
		auto get_par = [&](int m, int a) -> int {
			auto& par = pars[m];
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int m, int a, int b) -> bool {
			a = get_par(m, a);
			b = get_par(m, b);
			if (a == b) return false;
			auto& par = pars[m];
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};
		int64_t tot_ans = 0;
		for (int q = 0; q < Q; q++) {
			char op; cin >> op;
			if (op == '+') {
				int u, v, w; cin >> u >> v >> w; u--, v--;
				assert(w < V);
				[&](this auto&& self, int cur_w, int b) -> void {
					if (!merge(cur_w, u, v)) return;
					// look for the next bit
					for (; b < B; b++) {
						if (cur_w & (1 << b)) self(cur_w - (1 << b), b);
					}
				}(w, 0);
			} else if (op == '?') {
				int u, v; cin >> u >> v; u--, v--;
				int w;
				if (get_par(0, u) == get_par(0, v)) {
					w = 0;
					for (int b = B-1; b >= 0; b--) {
						int cnd = w + (1 << b);
						if (get_par(cnd, u) == get_par(cnd, v)) w = cnd;
					}
				} else {
					w = -1;
				}
				tot_ans += w;
			} else assert(false);
		}
		cout << tot_ans << '\n';
	}

	return 0;
}
