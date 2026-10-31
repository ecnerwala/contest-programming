#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;

		const int V = 2*((N-1) + (M-1));
		std::vector<int> par(V, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a);
			b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};

		for (int k = 0; k < K; k++) {
			int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
			x1--, y1--, x2--, y2--;
			assert(x2 - x1 == 1);
			assert(std::abs(y2 - y1) == 1);
			int u = 2 * x1;
			int v;
			if (y2 - y1 == 1) {
				v = 2 * ((N-1) + y1);
			} else if (y2 - y1 == -1) {
				v = 2 * ((N-1) + y2) + 1;
			} else assert(false);
			merge(u, v);
			merge(u^1, v^1);
		}
		bool good = true;
		for (int i = 0; i < V; i += 2) {
			if (get_par(i) == get_par(i+1)) {
				good = false;
				break;
			}
		}
		cout << (good ? "YES" : "NO") << '\n';
	}


	return 0;
}
